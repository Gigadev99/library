template<class T>
struct Vec2 {
    T x, y;
};

template<class T>
struct Vec3 {
    T x, y, z;
};

template<class T>
struct Vec4 {
    T x, y, z, t;
};

template<class T>
auto dot(const Vec2<T>& a, const Vec2<T>& b) {
    return a.x * b.x + a.y * b.y;
}

template<class T>
auto dot(const Vec3<T>& a, const Vec3<T>& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

template<class T>
auto dot(const Vec4<T>& a, const Vec4<T>& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.t * b.t;
}

template<class T>
Vec3<T> cross(const Vec3<T>& a, const Vec3<T>& b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}