#ifndef HABE09061_4FF2_4AC0_BEE8_238E22EB9896
#define HABE09061_4FF2_4AC0_BEE8_238E22EB9896

class Noncopyable
{
protected:
    Noncopyable(const Noncopyable&) = delete;
    Noncopyable& operator=(const Noncopyable&) = delete;
    Noncopyable() = default;
    ~Noncopyable() = default;
};

#endif /* HABE09061_4FF2_4AC0_BEE8_238E22EB9896 */
