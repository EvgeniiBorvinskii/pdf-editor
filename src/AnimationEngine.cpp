#include "AnimationEngine.h"
#include <cmath>

AnimationEngine::AnimationEngine(QObject *parent)
    : QObject(parent)
    , m_nextId(1)
    , m_lastFrameTime(0)
    , m_targetFPS(240)
{
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &AnimationEngine::tick);
}

AnimationEngine::~AnimationEngine() {
    stop();
}

int AnimationEngine::addAnimation(qreal duration, std::function<void(qreal)> update,
                                   Animation::EasingType easing) {
    auto animation = std::make_unique<Animation>();
    animation->id = m_nextId++;
    animation->duration = duration;
    animation->elapsed = 0.0;
    animation->update = update;
    animation->easing = easing;
    animation->active = true;
    
    int id = animation->id;
    m_animations.push_back(std::move(animation));
    
    return id;
}

void AnimationEngine::removeAnimation(int id) {
    m_animations.erase(
        std::remove_if(m_animations.begin(), m_animations.end(),
            [id](const auto& anim) { return anim->id == id; }),
        m_animations.end()
    );
}

void AnimationEngine::setFinishedCallback(int id, std::function<void()> callback) {
    for (auto& anim : m_animations) {
        if (anim->id == id) {
            anim->finished = callback;
            break;
        }
    }
}

void AnimationEngine::start() {
    m_elapsedTimer.start();
    m_lastFrameTime = 0;
    
    // Calculate interval for target FPS
    int interval = 1000 / m_targetFPS;
    m_timer->start(interval);
}

void AnimationEngine::stop() {
    m_timer->stop();
}

void AnimationEngine::setTargetFPS(int fps) {
    m_targetFPS = fps;
    if (m_timer->isActive()) {
        int interval = 1000 / m_targetFPS;
        m_timer->setInterval(interval);
    }
}

void AnimationEngine::tick() {
    qint64 currentTime = m_elapsedTimer.elapsed();
    qreal deltaTime = (currentTime - m_lastFrameTime) / 1000.0;
    m_lastFrameTime = currentTime;
    
    // Update all active animations
    for (auto it = m_animations.begin(); it != m_animations.end();) {
        auto& anim = *it;
        
        if (anim->active) {
            anim->elapsed += deltaTime;
            
            qreal progress = std::min(anim->elapsed / anim->duration, 1.0);
            qreal easedProgress = applyEasing(progress, anim->easing);
            
            // Call update callback
            if (anim->update) {
                anim->update(easedProgress);
            }
            
            // Check if animation is finished
            if (progress >= 1.0) {
                if (anim->finished) {
                    anim->finished();
                }
                it = m_animations.erase(it);
                continue;
            }
        }
        
        ++it;
    }
    
    emit frameRendered(deltaTime);
}

qreal AnimationEngine::applyEasing(qreal t, Animation::EasingType easing) {
    switch (easing) {
        case Animation::Linear:
            return t;
            
        case Animation::EaseInOut:
            return t < 0.5 
                ? 2.0 * t * t 
                : 1.0 - std::pow(-2.0 * t + 2.0, 2.0) / 2.0;
            
        case Animation::EaseIn:
            return t * t;
            
        case Animation::EaseOut:
            return 1.0 - (1.0 - t) * (1.0 - t);
            
        case Animation::Elastic: {
            const qreal c4 = (2.0 * M_PI) / 3.0;
            return t == 0.0 ? 0.0
                 : t == 1.0 ? 1.0
                 : std::pow(2.0, -10.0 * t) * std::sin((t * 10.0 - 0.75) * c4) + 1.0;
        }
            
        case Animation::Bounce: {
            const qreal n1 = 7.5625;
            const qreal d1 = 2.75;
            
            if (t < 1.0 / d1) {
                return n1 * t * t;
            } else if (t < 2.0 / d1) {
                t -= 1.5 / d1;
                return n1 * t * t + 0.75;
            } else if (t < 2.5 / d1) {
                t -= 2.25 / d1;
                return n1 * t * t + 0.9375;
            } else {
                t -= 2.625 / d1;
                return n1 * t * t + 0.984375;
            }
        }
            
        default:
            return t;
    }
}
