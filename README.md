# EventManager
---------

欢迎使用EventManager，一个非常简单易用的事件分发器。

### 背景
在游戏开发过程中，经常遇到这样一个问题。现在我们有几个系统任务系统，成就系统，这两个系统都需要处理玩家击杀怪物的事件。通常的做法就是在击杀怪物的处理函数中调用这个两个系统的对应接口，如下：
```
// Battle.cpp
void KillMonster(Player* player, Monster* monster)
{
    ...
    player->GetTaskMgr().OnKillMonster(monster);
    player->GetAchievementMgr().OnKillMonster(monster);
}
```

然后在对应接口中做各自的计数，状态变更等等等等。

某个时刻，来了一个新的需求，策划需要做一个活动，也需要处理这个事件。那么除了活动代码中需要增加接口`OnKillMonster`，`Battle.cpp`还需要做以下修改：
```
// Battle.cpp
void KillMonster(Player* player, Monster* monster)
{
    ...
    player->GetTaskMgr().OnKillMonster(monster);
    player->GetAchievementMgr().OnKillMonster(monster);
    player->GetActivityMgr().OnKillMonster(monster);
}
```

代码耦合度太高！

### 使用EventManager

我使用`C++`编写了这个`EventManager`。我们需要在`Event.h`中定义对应的事件：
```
struct KillMonsterEvent
{
    Monster* monster;
};
```

然后在`KillMonster`处理中发出这个事件:
```
// Battle.cpp
void KillMonster(Player* player, Monster* monster)
{
    ...
    KillMonsterEvent event{monster};
    FIRE_EVENT(event);
}
```

这个函数中的代码编写完成之后，今后不管什么功能需要这个事件，都需要再来修改这个函数了。新增系统的处理：
```
// ActivityMgr.cpp
class ActivityMgr
{
public:
    ActivityMgr()
    {
        REGISTER_EVENT(&ActivityMgr::OnKillMonster, this);
    }

    void OnKillMonster(Monster* monster)
    {
        // ...
    }
};
```

功能与功能之间解耦了。随着时间的推移，积累的事件越来越多，编写程序越来越方便。

现在只有在添加新的事件时，需要修改对应位置的代码。

详细用法查看`test.cpp`文件。

### 实现细节

* 非线程安全。
* `Any`类这里是自己实现的。实现参考祁 宇的《深入应用C++11：代码优化与工程级应用》一书。当然也可以使用`Boost`的`Any`库。
* 实现了一个简单的单例`Singleton`。
* 定义注册与发出事件的宏`REGISTER_EVENT`和`FIRE_EVENT`，方便使用。

### 要求

* 需要支持`C++11`的编译器。编译器支持情况可以看[这里](http://zh.cppreference.com/w/cpp/compiler_support)。