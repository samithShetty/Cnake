typedef char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef long long s64;
typedef unsigned long long u64;

#define global_variable static
#define internal static

int clamp(int min, int max, int value) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

template <typename T>
struct Queue {
    T* arr;
    int arrSize = 10;
    int headIndex;
    int tailIndex;
    unsigned int length;

    Queue() : headIndex(-1), tailIndex(0),length(0) {
        arr = (T*)malloc(arrSize * sizeof(T));
    }
    Queue(int s) : arrSize(s) {
        default;
    }

    void increaseCapacity() { // Allocate new array of double the size
        T* new_arr = (T*)malloc(arrSize * 2 * sizeof(T));
        // Shift array values to front and update head/tail indicies
        memcpy(new_arr, arr + tailIndex, sizeof(T) * (arrSize-tailIndex));
        memcpy(new_arr + arrSize - tailIndex, arr, sizeof(T) * tailIndex);
        tailIndex = 0;
        headIndex = arrSize - 1;
        free(arr);
        arr = new_arr;
        arrSize *= 2;
    }

    void push(T val) {
        if (length == arrSize) {
            increaseCapacity();
        }
        headIndex = (headIndex + 1) % arrSize;
        arr[headIndex] = val;
        length++;
    }

    T pop() {
        // Check if has elements
        if (length == 0) {
            throw;
        }
        T val = arr[tailIndex];
        tailIndex = (tailIndex + 1) % arrSize;
        length--;
        return val;
    }

    T getHead() {
        return arr[headIndex];
    }

};
