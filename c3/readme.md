# C3 (v0.7.10)

## It contains the wrong code, so please check it. (Created by copilot - Sonnet 4.6)
> C3은 C의 진화형 언어. C ABI 호환, 제로 오버헤드 추상화, 모던 문법이 특징.  
> 공식 사이트: https://c3-lang.org | 컴파일러: `c3c`

---

## 설치 & 기본 커맨드

```bash
# 설치 (Linux/macOS - 바이너리 직접 다운 또는 빌드)
git clone https://github.com/c3lang/c3c
cd c3c && cmake -B build && cmake --build build

# 기본 컴파일 & 실행
c3c compile main.c3            # 컴파��만
c3c run main.c3                # 컴파일 + 바로 실행
c3c compile -o myapp main.c3   # 출력 파일명 지정

# 프로젝트 (project.json 기반)
c3c init myproject             # 새 프로젝트 생성
c3c build                      # 프로젝트 빌드
c3c run                        # 프로젝트 실행

# 외부 라이브러리 링크
c3c compile main.c3 -l curl          # -l 로 시스템 라이브러리 링크
c3c compile main.c3 -L /usr/lib -l mylib  # 경로 지정

# 기타 옵션
c3c compile --target x86_64-linux main.c3   # 크로스 컴파일
c3c compile -O2 main.c3                      # 최적화
c3c compile --debug main.c3                  # 디버그 빌드
```

---

## 1. 기본 구조 & 모듈

```c3
// 모듈 선언 (파일 최상단)
module myapp;

// 표준 라이브러리 임포트
import std::io;
import std::math;
import std::collections::list;

fn int main(String[] args)
{
    io::printn("Hello, C3!");
    return 0;
}
```

---

## 2. 변수 & 타입

```c3
module types_demo;
import std::io;

fn void demo_types()
{
    // 기본 정수형
    int    a = 42;       // 32bit 부호있는 정수
    uint   b = 100;      // 32bit 부호없는
    long   c = 123456L;  // 64bit
    ulong  d = 999UL;
    short  e = 10;
    iptr   f = 0;        // 포인터 크기 정수

    // 명시적 크기
    uint128 big = 0;

    // 실수
    float  x = 3.14f;
    double y = 3.14159265;

    // 불리언
    bool flag = true;

    // char (= uint8)
    char ch = 'A';


    /* DOESN'T WORK
    // 타입 추론 (var)
    var auto_int = 10;       // int로 추론
    var auto_str = "hello";  // String으로 추론
    */
    // 상수
    const int MAX = 100;
    const double PI = 3.14159;

    // 캐스팅
    double result = (double)a / 3;
    int    truncated = (int)result;

    io::printfn("result: %f", result);
}
```

---

## 3. 문자열

```c3
module string_demo;
import std::io;

fn void demo_strings()
{
    // String: 불변 UTF-8 문자열 뷰 (슬라이스)
    String s = "Hello, World!";
    io::printfn("길이: %d", s.len);

    // 문자열 슬라이싱
    String sub = s[0..4];   // "Hello"
    io::printn(sub);

    // ZString: C 호��� null-terminated 문자열
    ZString zs = "C compatible";
    /* 
    // DString: 동적 가변 문자열 (힙 할당)
    DString ds = dstring::new("Hello");
    defer ds.free();         // 스코프 종료 시 자동 해제
    ds.append(", C3!");
    io::printn(ds.str_view());

    // 포매팅
    String formatted = string::format("%d + %d = %d", 1, 2, 3);
    defer formatted.free();  // format은 할당 발생
    io::printn(formatted);

    // 문자열 비교
    if (s == "Hello, World!") {
        io::printn("같음!");
    }

    // 포함 여부
    if (s.contains("World")) {
        io::printn("World 있음");
    }

    // 찾기 / 분리
    String upper = s.ascii_to_upper();
    defer upper.free();
    */
}
```

---

## 4. 배열 & 슬라이스

```c3
module array_demo;
import std::io;

fn void demo_arrays()
{
    // 고정 배열 (스�� 할당)
    int[5] arr = {1, 2, 3, 4, 5};

    // 배열 → 슬라이스
    int[] slice = arr[..];       // 전체
    int[] part  = arr[1..3];     // 인덱스 1~3

    // 길이
    io::printfn("길이: %d", slice.len);

    // foreach
    foreach (int v : arr) {
        io::printfn("%d", v);
    }

    // foreach with index
    foreach (usz i, int v : arr) {
        io::printfn("[%d] = %d", i, v);
    }

    // 2D 배열
    int[3][3] matrix = {{1,2,3},{4,5,6},{7,8,9}};

    // 동적 배열 → List 사용 (아래 컬렉션 섹션 참고)
}
```

---

## 5. 포인터 & 메모리

```c3
module pointer_demo;
import std::io;
import std::core::mem;

fn void demo_pointers()
{
    int x = 10;
    int* p = &x;      // 주소 취득
    *p = 20;          // 역참조
    io::printfn("x = %d", x);  // 20

    // 힙 할당
    int* heap = mem::new(int);
    defer mem::free(heap);
    *heap = 42;

    // 배열 힙 할당
    int* arr = mem::new_array(int, 5);
    defer mem::free(arr);
    arr[0] = 1;
    arr[1] = 2;

    // null 체크
    int* maybe = null;
    if (maybe != null) {
        io::printn("not null");
    }
}
```

---

## 6. 제어 흐름

```c3
module control_demo;
import std::io;

fn void demo_control()
{
    int x = 15;

    // if / else if / else
    if (x > 20) {
        io::printn("크다");
    } else if (x > 10) {
        io::printn("중간");  // 여기 출력
    } else {
        io::printn("작다");
    }

    // switch (C3은 기본 fallthrough 없음!)
    switch (x) {
        case 10:
            io::printn("10");
        case 15:
            io::printn("15");  // 여기만 실행
        case 20:
            io::printn("20");
        default:
            io::printn("기타");
    }

    // switch에서 fallthrough 하려면 명시적으로
    switch (x) {
        case 10:
        case 15:
            io::printn("10 또는 15");  // 두 케이스 공유
            nextcase;   // 다음 case로 fall through
        case 20:
            io::printn("15 또는 20 도달");
        default: break;
    }

    // for 루프
    for (int i = 0; i < 5; i++) {
        if (i == 3) continue;
        if (i == 4) break;
        io::printfn("i = %d", i);
    }

    // while
    int n = 0;
    while (n < 3) {
        io::printfn("n = %d", n);
        n++;
    }

    // do-while
    do {
        io::printn("한 번은 실행");
        n++;
    } while (n < 0);

    // 라벨 break (중첩 루프 탈출)
    OUTER: for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (i == 1 && j == 1) break OUTER;
            io::printfn("%d,%d", i, j);
        }
    }
}
```

---

## 7. 함수

```c3
module function_demo;
import std::io;

// 기본 함수
fn int add(int a, int b) {
    return a + b;
}

// void 함수
fn void greet(String name) {
    io::printfn("Hello, %s!", name);
}

// 다중 반환값 (구조체로)
struct DivResult {
    int quotient;
    int remainder;
}

fn DivResult divide(int a, int b) {
    return { .quotient = a / b, .remainder = a % b };
}

// 기본 인자값
fn void connect(String host, int port = 8080, bool ssl = false) {
    io::printfn("연결: %s:%d (ssl=%s)", host, port, ssl);
}

// 가변 인자
fn void log(String fmt, args...) {
    io::vprintf(fmt, args);
}

// 함수 포인터
def IntBinaryFn = fn int(int, int);

fn void use_fn_ptr() {
    IntBinaryFn op = &add;
    int result = op(3, 4);
    io::printfn("결과: %d", result);
}

// 인라인 함수
fn int square(int x) @inline {
    return x * x;
}

// 외부 C 함수 선언
extern fn int printf(ZString fmt, ...);

fn void main_demo() {
    io::printfn("3+4 = %d", add(3, 4));

    DivResult dr = divide(17, 5);
    io::printfn("17/5 = %d 나머지 %d", dr.quotient, dr.remainder);

    connect("localhost");            // port=8080, ssl=false
    connect("example.com", 443, true);

    use_fn_ptr();
}
```

---

## 8. struct & method

```c3
module struct_demo;
import std::io;
import std::math;

// 구조체 정의
struct Vec2 {
    double x;
    double y;
}

// 메서드 (구조체에 연결된 함수)
fn double Vec2.length(Vec2* self) {
    return math::sqrt(self.x * self.x + self.y * self.y);
}

fn void Vec2.print(Vec2* self) {
    io::printfn("Vec2(%f, %f)", self.x, self.y);
}

fn Vec2 Vec2.add(Vec2* self, Vec2 other) {
    return { .x = self.x + other.x, .y = self.y + other.y };
}

// 구조체 초기화 방법들
fn void demo_struct() {
    // 방법 1: 멤버 이름 지정
    Vec2 v1 = { .x = 3.0, .y = 4.0 };

    // 방법 2: 순서 지정
    Vec2 v2 = { 1.0, 2.0 };

    // 방법 3: 기본값 (0으로 초기화)
    Vec2 v3 = {};

    v1.print();
    io::printfn("길이: %f", v1.length());  // 5.0

    Vec2 sum = v1.add(v2);
    sum.print();

    // 중첩 구조체
    struct Player {
        String name;
        Vec2 position;
        int hp;
    }

    Player p = {
        .name = "Hero",
        .position = { .x = 10.0, .y = 5.0 },
        .hp = 100
    };
    io::printfn("%s HP:%d", p.name, p.hp);
}

// 비트 구조체
struct Flags @packed {
    bool read    : 1;
    bool write   : 1;
    bool execute : 1;
    uint padding : 5;
}
```

---

## 9. enum

```c3
module enum_demo;
import std::io;

// 기본 enum
enum Direction {
    NORTH,
    SOUTH,
    EAST,
    WEST
}

// 타입 지정 enum
enum Color : int {
    RED = 0xFF0000,
    GREEN = 0x00FF00,
    BLUE = 0x0000FF
}

// associated value enum (tagged union처럼)
enum Shape {
    CIRCLE,
    RECTANGLE,
    TRIANGLE
}

fn void describe(Shape s) {
    switch (s) {
        case CIRCLE:
            io::printn("원");
        case RECTANGLE:
            io::printn("사각형");
        case TRIANGLE:
            io::printn("삼각형");
    }
}

fn void demo_enum() {
    Direction dir = Direction.NORTH;

    // .name: enum 이름 문자열
    io::printn(dir.name);  // "NORTH"

    // .ordinal: 인덱스
    io::printfn("순서: %d", dir.ordinal);  // 0

    // enum 순회
    foreach (Direction d : Direction.values) {
        io::printn(d.name);
    }

    // switch (exhaustive - 모든 케이스 처리 강제)
    switch (dir) {
        case NORTH: io::printn("북쪽");
        case SOUTH: io::printn("남쪽");
        case EAST:  io::printn("동쪽");
        case WEST:  io::printn("서쪽");
    }

    describe(Shape.CIRCLE);
}
```

---

## 10. union

```c3
module union_demo;
import std::io;

union Data {
    int   i;
    float f;
    bool  b;
}

struct TaggedValue {
    int tag;  // 0=int, 1=float, 2=bool
    Data data;
}

fn void demo_union() {
    TaggedValue tv;
    tv.tag = 0;
    tv.data.i = 42;

    switch (tv.tag) {
        case 0: io::printfn("int: %d", tv.data.i);
        case 1: io::printfn("float: %f", tv.data.f);
        case 2: io::printfn("bool: %s", tv.data.b);
    }
}
```

---

## 11. Optional & 에러 처리

```c3
module error_demo;
import std::io;

// fault: 에러 타입 선언
fault MathError {
    DIVISION_BY_ZERO,
    OVERFLOW
}

fault FileError {
    NOT_FOUND,
    PERMISSION_DENIED
}

// Optional 반환 (실패 가능 함수)
// `int!` = int 또는 fault 반환
fn int! safe_divide(int a, int b)
{
    if (b == 0) return MathError.DIVISION_BY_ZERO?;
    return a / b;
}

// ? 연산자: 에러 전파 (Rust의 ? 와 유사)
fn int! compute(int x) 
{
    int result = safe_divide(100, x)!;  // 에러면 전파
    return result * 2;
}

fn void demo_errors()
{
    // try: 성공하면 값, 실패하면 null
    int? result = safe_divide(10, 2);
    if (try r = result) {
        io::printfn("결과: %d", r);
    }

    // catch: 에러 처리
    if (catch err = safe_divide(10, 0)) {
        io::printfn("에러 발생: %s", err);  // DIVISION_BY_ZERO
    }

    // 기본값 지정 (에러 시 0 반환)
    int val = safe_divide(10, 0) ?? 0;
    io::printfn("val = %d", val);  // 0

    // if (catch) 패턴
    int! r2 = safe_divide(10, 2);
    if (catch err = r2) {
        io::printfn("에러: %s", err);
    } else {
        io::printfn("성공: %d", r2);
    }
}
```

---

## 12. defer

```c3
module defer_demo;
import std::io;
import std::mem;

fn void demo_defer()
{
    // 역순 실행 (LIFO)
    defer io::printn("3번째 출력");
    defer io::printn("2번째 출력");
    defer io::printn("1번째 출력");

    io::printn("함수 본체");
    // 출력 순서: 함수 본체 → 1번째 → 2번째 → 3번째
}

// 실용적 사용: 리소스 해제
fn void read_file(String path)
{
    // 파일 열기 → 에러 처리
    File! f = file::open(path, "r");
    if (catch err = f) {
        io::printfn("파일 열기 실패: %s", err);
        return;
    }
    defer f.close();  // 함수 종료 시 자동 닫기

    // 메모리 할당 + 자동 해제
    char* buf = mem::new_array(char, 1024);
    defer mem::free(buf);

    // ... 파일 처리 ...
    io::printn("파일 처리 중");
}
// f.close(), mem::free(buf) 자동 호출됨
```

---

## 13. 제네릭 

```c3
module generic_demo;
import std::io;

// 제네릭 함수
fn T max(T)(T a, T b) {
    return a > b ? a : b;
}

// 제네릭 구조체
struct Pair<A, B> {
    A first;
    B second;
}

fn void Pair.print(<A, B>)(Pair(<A, B>)* self) {
    // 타입별 출력 (실제론 anycast 또는 별도 처리)
    io::printfn("Pair(%s, %s)", self.first, self.second);
}

// 제네릭 Stack 예시
struct Stack<T> {
    T[100] data;
    int top;
}

fn void Stack.push(Stack* self, T val) {
    self.data[self.top++] = val;
}

fn T Stack.pop(Stack* self) {
    return self.data[--self.top];
}

fn bool Stack.empty(Stack* self) {
    return self.top == 0;
}

fn void demo_generics() {
    io::printfn("max(3,5) = %d", max{int}(3, 5));
    io::printfn("max(1.2, 3.4) = %f", max{double}(1.2, 3.4));

    Stack{int} s;
    s.top = 0;
    s.push(10);
    s.push(20);
    s.push(30);
    io::printfn("pop: %d", s.pop());  // 30
    io::printfn("pop: %d", s.pop());  // 20
}
```

---

## 14. 표준 라이브러리 - Collections

```c3
module collections_demo;
import std::io;
import std::collections::list;
import std::collections::map;

fn void demo_list()
{
    // List (동적 배열)
    List(<int>) nums;
    nums.init();
    defer nums.free();

    nums.push(1);
    nums.push(2);
    nums.push(3);
    nums.push(10);

    io::printfn("크기: %d", nums.len());

    // 인덱스 접근
    io::printfn("첫번째: %d", nums[0]);

    // 순회
    foreach (int v : nums) {
        io::printfn("%d", v);
    }

    // 삭제
    nums.remove_at(1);  // 인덱스 1 삭제

    // 정렬
    nums.sort();
}

fn void demo_map()
{
    // HashMap
    HashMap(<String, int>) scores;
    scores.init();
    defer scores.free();

    scores["Alice"] = 95;
    scores["Bob"]   = 87;
    scores["Charlie"] = 92;

    // 조회
    if (try val = scores.get("Alice")) {
        io::printfn("Alice: %d", val);
    }

    // 키 존재 확인
    if (scores.has_key("Bob")) {
        io::printn("Bob 있음");
    }

    // 순회
    foreach (String k, int v : scores) {
        io::printfn("%s -> %d", k, v);
    }

    // 삭제
    scores.remove("Bob");
}
```

---

## 15. 인터페이스 (interface)

```c3
module interface_demo;
import std::io;
import std::math;

// 인터페이스 정의
interface Drawable {
    fn void draw(self);
    fn double area(self);
}

struct Circle {
    double radius;
}

struct Rectangle {
    double width;
    double height;
}

// Circle이 Drawable 구현
fn void Circle.draw(Circle* self) {
    io::printfn("원 (반지름: %f)", self.radius);
}

fn double Circle.area(Circle* self) {
    return math::PI * self.radius * self.radius;
}

// Rectangle이 Drawable 구현
fn void Rectangle.draw(Rectangle* self) {
    io::printfn("사각형 (%f x %f)", self.width, self.height);
}

fn double Rectangle.area(Rectangle* self) {
    return self.width * self.height;
}

// 인터페이스 타입으로 다형성
fn void print_shape(Drawable shape) {
    shape.draw();
    io::printfn("넓이: %f", shape.area());
}

fn void demo_interface() {
    Circle c = { .radius = 5.0 };
    Rectangle r = { .width = 4.0, .height = 6.0 };

    print_shape(&c);
    print_shape(&r);
}
```

---

## 16. `any` 타입 & 타입 검사

```c3
module any_demo;
import std::io;

fn void process(any value)
{
    // 타입 확인
    switch (value) {
        case int i:
            io::printfn("int: %d", i);
        case double d:
            io::printfn("double: %f", d);
        case String s:
            io::printfn("string: %s", s);
        default:
            io::printn("알 수 없는 타입");
    }
}

fn void demo_any() {
    process(42);
    process(3.14);
    process("hello");
}
```

---

## 17. 매크로 (`macro`)

```c3
module macro_demo;
import std::io;

// 매크로 정의 (@로 시작하는 규칙)
macro void @swap(#a, #b) {
    var tmp = a;
    a = b;
    b = tmp;
}

macro int @max(a, b) {
    return a > b ? a : b;
}

// 컴파일 타임 조건
macro void @assert_debug(#cond, #msg) {
    $if $defined(DEBUG):
        if (!cond) io::printn(msg);
    $endif
}

fn void demo_macro() {
    int x = 3, y = 7;
    @swap(x, y);
    io::printfn("x=%d y=%d", x, y);  // x=7 y=3

    io::printfn("max: %d", @max(x, y));
}
```

---

## 18. C 라이브러리 연동

```c3
module c_interop;
import std::io;

// C 함수 선언 (extern)
extern fn int printf(ZString fmt, ...);
extern fn void* malloc(usz size);
extern fn void  free(void* ptr);
extern fn int   strlen(ZString s);

// C 구조체 호환
struct CPoint @cname("Point") {
    int x;
    int y;
}

fn void demo_c_interop()
{
    // C printf 직접 호출
    printf("C printf: %d\n", 42);

    // malloc/free
    int* buf = (int*)malloc(10 * int.sizeof);
    defer free(buf);
    buf[0] = 123;
    printf("buf[0] = %d\n", buf[0]);

    // strlen
    ZString zs = "hello";
    int len = strlen(zs);
    printf("len = %d\n", len);
}
```

### 외부 라이브러리 연동 (예: libcurl)

```c3
// curl_binding.c3 - 바인딩 파일
module curl;

// 헤더의 타입/함수 수동 선언
def CurlHandle = void*;
const int CURLOPT_URL = 10002;

extern fn CurlHandle curl_easy_init();
extern fn int curl_easy_setopt(CurlHandle handle, int option, ...);
extern fn int curl_easy_perform(CurlHandle handle);
extern fn void curl_easy_cleanup(CurlHandle handle);
```

```c3
// main.c3
module main;
import std::io;
import curl;

fn void fetch(String url)
{
    CurlHandle handle = curl.curl_easy_init();
    if (handle == null) {
        io::printn("curl 초기화 실패");
        return;
    }
    defer curl.curl_easy_cleanup(handle);

    curl.curl_easy_setopt(handle, curl.CURLOPT_URL, url.ptr);
    int res = curl.curl_easy_perform(handle);
    io::printfn("결과 코드: %d", res);
}
```

```bash
# 빌드 시 라이브러리 링크
c3c compile main.c3 curl_binding.c3 -l curl -o fetcher
```

---

## 19. 프로젝트 구조 & `project.json`

```json
{
  "name": "myproject",
  "version": "0.1.0",
  "sources": ["src/**"],
  "c-sources": ["csrc/**"],
  "libs": ["vendor/mylib"],
  "linked-libraries": ["curl", "pthread"],
  "warnings": ["no-unused"],
  "optimization": "O2"
}
```

```bash
myproject/
├── project.json      # 프로젝트 설정
├── src/
│   ├── main.c3
│   └── utils.c3
├── lib/              # C3 라이브러리
└── vendor/           # 외부 라이브러리
```

```bash
c3c init myproject    # 프로젝트 초기화
c3c build             # 빌드
c3c run               # 실행
c3c test              # 테스트
c3c clean             # 빌드 결과 삭제
```

---

## 핵심 문법 요약표

| 개념 | 문법 |
|---|---|
| 변수 선언 | `int x = 5;` / `var x = 5;` |
| 상수 | `const int MAX = 100;` |
| 함수 | `fn int add(int a, int b) { return a+b; }` |
| 구조체 | `struct Foo { int x; String s; }` |
| 메서드 | `fn void Foo.method(Foo* self) {}` |
| 열거형 | `enum Dir { NORTH, SOUTH }` |
| 에러 타입 | `fault MyError { NOT_FOUND, FAILED }` |
| Optional | `fn int! safe_fn() { ... }` |
| 에러 전파 | `int val = risky()!;` |
| 에러 기본값 | `int val = risky() ?? 0;` |
| defer | `defer resource.free();` |
| 제네릭 | `fn T max(T)(T a, T b)` |
| 인터페이스 | `interface Drawable { fn void draw(self); }` |
| any 타입 | `fn void f(any v) { switch(v) { case int i: ... } }` |
| 슬라이스 | `int[] s = arr[1..3];` |
| foreach | `foreach (int v : arr) {}` |
| 포인터 | `int* p = &x; *p = 10;` |
| C 연동 | `extern fn int printf(ZString fmt, ...);` |

---

> **참고 링크**
> - 공식 문서: https://c3-lang.org
> - 튜토리얼: https://learn-c3.org
> - GitHub: https://github.com/c3lang/c3c
> - 예제 모음: https://github.com/o3o/c3-doc-code-examples
