#include <bits/stdc++.h>

#if defined(_WIN32)
#include <windows.h>
#elif defined(__linux__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__) || defined(__APPLE__)
#include <strings.h>
#endif

// using const_bytes_span = std::span<const uint8_t>;
using namespace std;

template <typename T>
class simple_span
{
public:
    simple_span() : data_(nullptr), size_(0) {}

    simple_span(T *d, std::size_t s) : data_(d), size_(s) {}

    simple_span(std::initializer_list<typename std::remove_const<T>::type> il)
        : data_(il.begin()), size_(il.size()) {}

    template <std::size_t N>
    simple_span(std::array<typename std::remove_const<T>::type, N> &arr)
        : data_(arr.data()), size_(N) {}

    template <std::size_t N>
    simple_span(const std::array<typename std::remove_const<T>::type, N> &arr)
        : data_(arr.data()), size_(N) {}

    simple_span(const std::string &str)
        : data_(reinterpret_cast<T *>(str.data())), size_(str.size()) {}

    simple_span(const std::vector<typename std::remove_const<T>::type> &vec)
        : data_(vec.data()), size_(vec.size()) {}

    T *data() const { return data_; }
    std::size_t size() const { return size_; }
    std::size_t size_bytes() const { return size_ * sizeof(T); }
    bool empty() const { return size_ == 0; }

    T *begin() const { return data_; }
    T *end() const { return data_ + size_; }

    T &operator[](std::size_t idx) const { return data_[idx]; }

    simple_span subspan(std::size_t offset, std::size_t count = static_cast<std::size_t>(-1)) const
    {
        if (count == static_cast<std::size_t>(-1))
        {
            count = size_ - offset;
        }
        return simple_span(data_ + offset, count);
    }

private:
    T *data_;
    std::size_t size_;
};

using const_bytes_span = simple_span<const uint8_t>;

namespace 
{
inline void secure_memzero(void *ptr, std::size_t size)
    {
        if (ptr == nullptr || size == 0)
        {
            return;
        }

#if defined(__cpp_lib_memset_explicit) && __cpp_lib_memset_explicit >= 202207L
        // C++ standard secure memory zero
        std::memset_explicit(ptr, 0, size);
#elif defined(_WIN32)
        // Windows-specific secure memory zero
        SecureZeroMemory(ptr, size);
#elif defined(__linux__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__) || defined(__APPLE__)
        // POSIX-specific secure memory zero (glibc 2.25+, BSD, macOS)
        explicit_bzero(ptr, size);
#else
        // Portable fallback: Use a volatile pointer loop.
        volatile unsigned char *p = static_cast<volatile unsigned char *>(ptr);
        while (size--)
        {
            *p++ = 0;
        }
#endif
}

template <typename T>
inline void mem_wipe(T *ptr, std::size_t count)
{
    secure_memzero(static_cast<void *>(ptr), count * sizeof(T));
}

template <typename T>
inline void mem_wipe(T &obj)
{
    secure_memzero(static_cast<void *>(&obj), sizeof(T));
}

template <typename T>
inline void mem_wipe(simple_span<T> s)
{
    secure_memzero(s.data(), s.size_bytes());
}

inline bool secure_compare(const_bytes_span a, const_bytes_span b)
{
    if (a.size() != b.size())
    {
        return false;
    }
    uint8_t diff = 0;
    for (std::size_t i = 0; i < a.size(); ++i)
    {
        diff |= (a[i] ^ b[i]);
    }
    return diff == 0;
}

inline uint32_t rotr(uint32_t n, unsigned int c)
{
    return (n >> c) | (n << (32 - c));
}

inline uint32_t byteswap32(uint32_t x)
{
#if defined(_MSC_VER)
    return _byteswap_ulong(x);
#elif defined(__GNUC__) || defined(__clang__)
    return __builtin_bswap32(x);
#else
    return ((x << 24) & 0xff000000) | ((x << 8) & 0x00ff0000) |
           ((x >> 8) & 0x0000ff00) | ((x >> 24) & 0x000000ff);
#endif
}

inline uint64_t byteswap64(uint64_t x)
{
#if defined(_MSC_VER)
    return _byteswap_uint64(x);
#elif defined(__GNUC__) || defined(__clang__)
    return __builtin_bswap64(x);
#else
    return ((x << 56) & 0xff00000000000000ULL) | ((x << 40) & 0x00ff000000000000ULL) |
           ((x << 24) & 0x0000ff0000000000ULL) | ((x << 8) & 0x000000ff00000000ULL) |
           ((x >> 8) & 0x00000000ff000000ULL) | ((x >> 24) & 0x0000000000ff0000ULL) |
           ((x >> 40) & 0x000000000000ff00ULL) | ((x >> 56) & 0x00000000000000ffULL);
#endif
}

template <typename T>
const_bytes_span as_bytes(const T &obj)
{
    static_assert(std::is_standard_layout_v<T> && std::is_trivially_default_constructible_v<T> && std::is_trivially_copyable_v<T>,
                  "Object must be standard layout and trivial to be viewed as bytes.");
    return const_bytes_span(reinterpret_cast<const uint8_t *>(&obj), sizeof(T));
}

inline std::string to_hex(const_bytes_span data)
{
    static constexpr std::string_view hex_chars = "0123456789abcdef";
    std::string res;
    res.reserve(data.size() * 2);
    for (const uint8_t b : data)
    {
        res.push_back(hex_chars[(b >> 4) & 0x0F]);
        res.push_back(hex_chars[b & 0x0F]);
    }
    return res;
}

} // namespace 




namespace
{

    inline uint32_t ch(uint32_t x, uint32_t y, uint32_t z)
    {
        return (x & y) ^ (~x & z);
    }
    inline uint32_t maj(uint32_t x, uint32_t y, uint32_t z)
    {
        return (x & y) ^ (x & z) ^ (y & z);
    }
    inline uint32_t ep0(uint32_t x)
    {
        return rotr(x, 2) ^ rotr(x, 13) ^ rotr(x, 22);
    }
    inline uint32_t ep1(uint32_t x)
    {
        return rotr(x, 6) ^ rotr(x, 11) ^ rotr(x, 25);
    }
    inline uint32_t sig0(uint32_t x)
    {
        return rotr(x, 7) ^ rotr(x, 18) ^ (x >> 3);
    }
    inline uint32_t sig1(uint32_t x)
    {
        return rotr(x, 17) ^ rotr(x, 19) ^ (x >> 10);
    }

    constexpr std::array<uint32_t, 64> K = {0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
                                            0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
                                            0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
                                            0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
                                            0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
                                            0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
                                            0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
                                            0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};
} // namespace

class Sha256
{
public:
    static const size_t BLOCK_SIZE = 64;
    static const size_t HASH_SIZE = 32;

    using Digest = std::array<uint8_t, HASH_SIZE>;

    Sha256();
    ~Sha256();

    Sha256(const Sha256 &) = delete;
    Sha256 &operator=(const Sha256 &) = delete;

    Sha256(Sha256 &&) noexcept = default;
    Sha256 &operator=(Sha256 &&) noexcept = default;

    void update(const_bytes_span data);
    Digest finalize();

    static Digest hash(const_bytes_span data);
    static bool verify(const_bytes_span data, const Digest &expected);

private:
    static void process_block(std::array<uint32_t, 8> & state, const uint8_t *block);

    std::array<uint32_t, 8> state_;
    std::array<uint8_t, BLOCK_SIZE> buffer_;
    uint64_t total_bytes_;
    bool finalized_;
};

Sha256::Sha256()
    : state_{0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19}, buffer_{}, total_bytes_(0), finalized_(false)
{
}

Sha256::~Sha256()
{
    mem_wipe(state_);
    mem_wipe(buffer_);
}

void Sha256::process_block(std::array<uint32_t, 8> & state, const uint8_t *block)
{
    std::array<uint32_t, 64> w{};

    for (size_t i = 0; i < 16; ++i)
    {
        uint32_t word = 0;
        std::memcpy(&word, block + i * 4, 4);
        w[i] = byteswap32(word);
    }

    for (size_t i = 16; i < 64; ++i)
    {
        w[i] = w[i - 16] + sig0(w[i - 15]) + w[i - 7] + sig1(w[i - 2]);
    }

    uint32_t a = state[0];
    uint32_t b = state[1];
    uint32_t c = state[2];
    uint32_t d = state[3];
    uint32_t e = state[4];
    uint32_t f = state[5];
    uint32_t g = state[6];
    uint32_t h = state[7];

    for (int i = 0; i < 64; ++i)
    {
        uint32_t T1 = h + ep1(e) + ch(e, f, g) + K[i] + w[i];
        uint32_t T2 = ep0(a) + maj(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + T1;
        d = c;
        c = b;
        b = a;
        a = T1 + T2;
    }

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
    state[5] += f;
    state[6] += g;
    state[7] += h;

    mem_wipe(w);
}

void Sha256::update(const_bytes_span data)
{
    if (finalized_)
    {
        return;
    }

    size_t offset = total_bytes_ % BLOCK_SIZE;
    total_bytes_ += data.size();

    if (offset > 0)
    {
        size_t chunk = std::min(data.size(), BLOCK_SIZE - offset);
        std::memcpy(buffer_.data() + offset, data.data(), chunk);
        data = data.subspan(chunk);
        offset = (offset + chunk) % BLOCK_SIZE;

        if (offset == 0)
        {
            process_block(state_, buffer_.data());
        }
    }

    while (data.size() >= BLOCK_SIZE)
    {
        process_block(state_, data.data());
        data = data.subspan(BLOCK_SIZE);
    }

    if (!data.empty())
    {
        std::memcpy(buffer_.data(), data.data(), data.size());
    }
}

Sha256::Digest Sha256::finalize()
{
    if (!finalized_)
    {
        const uint64_t bit_len = total_bytes_ * 8;
        const uint8_t pad = 0x80;
        update({&pad, 1});

        while (total_bytes_ % BLOCK_SIZE != 56)
        {
            const uint8_t zero = 0x00;
            update({&zero, 1});
        }

        uint64_t be_len = byteswap64(bit_len);
        update({reinterpret_cast<const uint8_t *>(&be_len), 8});
        finalized_ = true;
    }

    Digest res;
    for (int i = 0; i < 8; ++i)
    {
        uint32_t word = byteswap32(state_[i]);
        std::memcpy(res.data() + (i * 4), &word, 4);
    }
    return res;
}

Sha256::Digest Sha256::hash(const_bytes_span data)
{
    Sha256 ctx;
    ctx.update(data);
    return ctx.finalize();
}

bool Sha256::verify(const_bytes_span data, const Digest &expected)
{
    return secure_compare(hash(data), expected);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    Sha256 ctx;
    const_bytes_span chunk_span(s);
    
    ctx.update(chunk_span);
    
    Sha256::Digest digest = ctx.finalize();
    std::string actual = to_hex(as_bytes(digest));
    cout << actual << endl;
    return 0;
}