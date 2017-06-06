#ifndef __VDEV_HPP
#define __VDEV_HPP

#define DEFINE_DEVICE(NAME)                     \
    using VDEVICE_CLASS_NAME = NAME;

#define DEFINE_RO_PROPERTY(TYPE, NAME, HANDLER) \
    inline TYPE get_##NAME() {                  \
        return HANDLER();                       \
    }                                           \
    struct {                                    \
        VDEVICE_CLASS_NAME& owner_;             \
        inline operator TYPE () {               \
            return owner_.get_##NAME();         \
        }                                       \
    } NAME{*this};

#define DEFINE_WO_PROPERTY(TYPE, NAME, HANDLER)         \
    inline void set_##NAME(const TYPE& v) {             \
        return HANDLER(v);                              \
    }                                                   \
    struct {                                            \
        VDEVICE_CLASS_NAME& owner_;                     \
        inline void operator = (const TYPE& v) {        \
            owner_.set_##NAME(v);                       \
        }                                               \
    } NAME{*this};

#define DEFINE_RW_PROPERTY(TYPE, NAME, READER, WRITER)  \
    inline TYPE get_##NAME() {                          \
        return READER();                                \
    }                                                   \
    inline void set_##NAME(const TYPE& v) {             \
        return WRITER(v);                               \
    }                                                   \
    struct {                                            \
        VDEVICE_CLASS_NAME& owner_;                     \
        inline operator TYPE () {                       \
            return owner_.get_##NAME();                 \
        }                                               \
        inline void operator = (const TYPE& v) {        \
            owner_.set_##NAME(v);                       \
        }                                               \
    } NAME{*this};

#endif // __VDEV_HPP
