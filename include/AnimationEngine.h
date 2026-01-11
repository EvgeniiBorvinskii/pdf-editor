#ifndef ANIMATIONENGINE_H
#define ANIMATIONENGINE_H

#include <QObject>
#include <QTimer>
#include <QElapsedTimer>
#include <functional>
#include <vector>
#include <memory>

class AnimationEngine : public QObject {
    Q_OBJECT

public:
    struct Animation {
        int id;
        qreal duration;
        qreal elapsed;
        std::function<void(qreal)> update;
        std::function<void()> finished;
        bool active;
        
        enum EasingType {
            Linear,
            EaseInOut,
            EaseIn,
            EaseOut,
            Elastic,
            Bounce
        } easing;
    };

    explicit AnimationEngine(QObject *parent = nullptr);
    ~AnimationEngine();

    int addAnimation(qreal duration, std::function<void(qreal)> update, 
                     Animation::EasingType easing = Animation::EaseInOut);
    void removeAnimation(int id);
    void setFinishedCallback(int id, std::function<void()> callback);
    
    void start();
    void stop();
    void setTargetFPS(int fps);

signals:
    void frameRendered(qreal deltaTime);

private slots:
    void tick();

private:
    qreal applyEasing(qreal t, Animation::EasingType easing);
    
    QTimer *m_timer;
    QElapsedTimer m_elapsedTimer;
    std::vector<std::unique_ptr<Animation>> m_animations;
    int m_nextId;
    qint64 m_lastFrameTime;
    int m_targetFPS;
};

#endif // ANIMATIONENGINE_H
