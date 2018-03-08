template <typename T>
class Singleton
{
public:
    Singleton() {}
    ~Singleton() {}

    static T& getSingleton()
    {
        static T s_singleton;
        return s_singleton;
    }

private:
    Singleton(const Singleton&) = delete;
    const Singleton & operator= (const Singleton &) = delete;
};