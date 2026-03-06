
# 🦎 Zig 언어 정리

## 1. 기본 구조 & Hello World

```zig name=hello.zig
const std = @import("std");

pub fn main() void {
    std.debug.print("Hello, {s}!\n", .{"World"});
}
```

---

## 2. 변수 선언

```zig name=variables.zig
const std = @import("std");

pub fn main() void {
    // const: 컴파일 타임 상수 (불변)
    const x: i32 = 10;

    // var: 런타임 변수 (변경 가능)
    var y: i32 = 20;
    y += 5;

    // 타입 추론 (comptime)
    const z = 42; // i32로 추론

    // undefined: 초기화 안 함 (나중에 반드시 써야 함)
    var buf: [10]u8 = undefined;
    buf[0] = 'A';

    std.debug.print("x={}, y={}, z={}\n", .{ x, y, z });
}
```

---

## 3. 기본 타입

```zig name=types.zig
const std = @import("std");

pub fn main() void {
    // 정수형
    const a: i8   = -128;
    const b: u8   = 255;
    const c: i16  = 1000;
    const d: u32  = 999999;
    const e: i64  = -9999999999;
    const f: u64  = 18446744073709551615;
    const g: i128 = 99999999999999999999;
    const h: isize = -1; // 포인터 크기 부호 있는 정수
    const i_: usize = 42; // 포인터 크기 부호 없는 정수 (배열 인덱스용)

    // 부동소수점
    const pi: f32  = 3.14;
    const tau: f64 = 6.283185307;
    const big: f128 = 1.23456789012345678901234567890;

    // bool
    const flag: bool = true;

    // 문자 (u8)
    const ch: u8 = 'Z';

    // comptime_int / comptime_float (컴파일 타임 전용 숫자)
    const ct: comptime_int = 1000000;

    // void
    const nothing: void = {};

    std.debug.print("{} {} {} {} {} {} {} {} {}\n",
        .{ a, b, c, d, e, f, g, h, i_ });
    std.debug.print("pi={} tau={} big={}\n", .{ pi, tau, big });
    std.debug.print("flag={} ch={c} ct={} nothing={}\n",
        .{ flag, ch, ct, nothing });
}
```

---

## 4. 포인터

```zig name=pointers.zig
const std = @import("std");

pub fn main() void {
    var x: i32 = 10;

    // 단일 포인터 (*T)
    const ptr: *i32 = &x;
    ptr.* = 99;  // 역참조
    std.debug.print("x = {}\n", .{x}); // 99

    // const 포인터 (*const T): 값을 읽기만 가능
    const cptr: *const i32 = &x;
    std.debug.print("cptr = {}\n", .{cptr.*});

    // 선택적 포인터 (?*T)
    var maybe: ?*i32 = null;
    maybe = &x;
    if (maybe) |p| {
        std.debug.print("maybe = {}\n", .{p.*});
    }

    // 다중 포인터 ([*]T): 배열처럼 포인터 산술 가능
    var arr = [_]i32{ 1, 2, 3 };
    const mptr: [*]i32 = &arr;
    std.debug.print("{} {} {}\n", .{ mptr[0], mptr[1], mptr[2] });
}
```

---

## 5. 슬라이스 & 배열

```zig name=slices_arrays.zig
const std = @import("std");

pub fn main() void {
    // 배열: 고정 크기, 컴파일 타임에 크기 확정
    const arr = [5]i32{ 10, 20, 30, 40, 50 };
    std.debug.print("arr len = {}\n", .{arr.len}); // 5

    // _로 크기 추론
    const arr2 = [_]u8{ 'H', 'i', '!' };
    std.debug.print("{s}\n", .{arr2});

    // 슬라이스: 포인터 + 길이, 런타임 크기
    const slice: []const i32 = arr[1..4]; // {20, 30, 40}
    std.debug.print("slice[0]={} len={}\n", .{ slice[0], slice.len });

    // 문자열은 []const u8 슬라이스
    const s: []const u8 = "Hello, Zig!";
    std.debug.print("{s} len={}\n", .{ s, s.len });

    // 가변 슬라이스
    var buf = [_]u8{ 0, 0, 0, 0, 0 };
    var mslice: []u8 = buf[0..];
    mslice[0] = 'Z';
    mslice[1] = 'i';
    mslice[2] = 'g';
    std.debug.print("{s}\n", .{mslice[0..3]});
}
```

---

## 6. 제어 흐름

```zig name=control_flow.zig
const std = @import("std");

pub fn main() void {
    const n = 7;

    // if / else if / else
    if (n < 0) {
        std.debug.print("negative\n", .{});
    } else if (n == 0) {
        std.debug.print("zero\n", .{});
    } else {
        std.debug.print("positive\n", .{});
    }

    // if는 표현식 (값을 반환)
    const label = if (n % 2 == 0) "even" else "odd";
    std.debug.print("{s}\n", .{label});

    // while
    var i: usize = 0;
    while (i < 5) : (i += 1) {
        std.debug.print("i={}\n", .{i});
    }

    // while with continue condition
    var j: usize = 0;
    while (j < 10) : (j += 2) {
        if (j == 6) break;
        std.debug.print("j={}\n", .{j});
    }

    // for (배열/슬라이스 순회)
    const nums = [_]i32{ 1, 2, 3, 4, 5 };
    for (nums) |val| {
        std.debug.print("val={}\n", .{val});
    }

    // for with index
    for (nums, 0..) |val, idx| {
        std.debug.print("[{}]={}\n", .{ idx, val });
    }

    // for range (Zig 0.12+)
    for (0..5) |k| {
        std.debug.print("k={}\n", .{k});
    }

    // switch
    const day = 3;
    const name = switch (day) {
        1 => "Mon",
        2 => "Tue",
        3 => "Wed",
        4, 5 => "Thu or Fri",
        6...7 => "Weekend",
        else => "Unknown",
    };
    std.debug.print("{s}\n", .{name});
}
```

---

## 7. 함수

```zig name=functions.zig
const std = @import("std");

// 기본 함수
fn add(a: i32, b: i32) i32 {
    return a + b;
}

// 여러 값 반환 (구조체 or 튜플로)
fn divmod(a: i32, b: i32) struct { q: i32, r: i32 } {
    return .{ .q = @divTrunc(a, b), .r = @mod(a, b) };
}

// 에러 반환 함수
fn safeDivide(a: i32, b: i32) !i32 {
    if (b == 0) return error.DivisionByZero;
    return @divTrunc(a, b);
}

// comptime 파라미터 (제네릭)
fn printValue(comptime T: type, val: T) void {
    std.debug.print("value: {}\n", .{val});
}

// 재귀
fn factorial(n: u64) u64 {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

pub fn main() !void {
    std.debug.print("add = {}\n", .{add(3, 4)});

    const dm = divmod(17, 5);
    std.debug.print("q={} r={}\n", .{ dm.q, dm.r });

    const result = try safeDivide(10, 3);
    std.debug.print("safe = {}\n", .{result});

    // 에러 처리
    const err_result = safeDivide(10, 0);
    if (err_result) |v| {
        std.debug.print("ok = {}\n", .{v});
    } else |err| {
        std.debug.print("error: {}\n", .{err});
    }

    printValue(i32, 42);
    printValue(f64, 3.14);

    std.debug.print("10! = {}\n", .{factorial(10)});
}
```

---

## 8. 구조체 (struct)

```zig name=structs.zig
const std = @import("std");

// 구조체 정의
const Point = struct {
    x: f64,
    y: f64,

    // 메서드
    pub fn distance(self: Point, other: Point) f64 {
        const dx = self.x - other.x;
        const dy = self.y - other.y;
        return @sqrt(dx * dx + dy * dy);
    }

    // 생성자 패턴
    pub fn init(x: f64, y: f64) Point {
        return .{ .x = x, .y = y };
    }
};

// 팩드 구조체 (메모리 최적화)
const Flags = packed struct {
    read: bool,
    write: bool,
    exec: bool,
    _pad: u5 = 0,
};

// extern 구조체 (C ABI 호환)
const CStruct = extern struct {
    id: c_int,
    value: c_float,
};

pub fn main() void {
    const p1 = Point.init(0, 0);
    const p2 = Point{ .x = 3, .y = 4 };
    std.debug.print("distance = {}\n", .{p1.distance(p2)}); // 5.0

    var flags = Flags{ .read = true, .write = true, .exec = false };
    std.debug.print("read={} write={} exec={}\n",
        .{ flags.read, flags.write, flags.exec });

    // 익명 구조체 (튜플처럼)
    const t = .{ 1, "hello", true };
    std.debug.print("{} {s} {}\n", .{ t[0], t[1], t[2] });
}
```

---

## 9. 열거형 (enum)

```zig name=enums.zig
const std = @import("std");

const Direction = enum {
    north,
    south,
    east,
    west,

    pub fn opposite(self: Direction) Direction {
        return switch (self) {
            .north => .south,
            .south => .north,
            .east  => .west,
            .west  => .east,
        };
    }
};

// 정수 태그가 있는 enum
const Status = enum(u8) {
    ok    = 200,
    notFound = 404,
    error_  = 500,
};

pub fn main() void {
    const dir = Direction.north;
    std.debug.print("opposite = {}\n", .{dir.opposite()});

    // enum → 정수
    const s = Status.ok;
    std.debug.print("status = {}\n", .{@intFromEnum(s)}); // 200

    // 정수 → enum
    const s2 = @as(Status, @enumFromInt(404));
    std.debug.print("s2 = {}\n", .{s2});

    // switch로 enum 패턴 매칭
    switch (dir) {
        .north, .south => std.debug.print("vertical\n", .{}),
        .east, .west   => std.debug.print("horizontal\n", .{}),
    }
}
```

---

## 10. 유니온 (union)

```zig name=unions.zig
const std = @import("std");

// 태그 유니온 (tagged union) - 가장 많이 쓰임
const Shape = union(enum) {
    circle:    f64, // 반지름
    rectangle: struct { w: f64, h: f64 },
    triangle:  struct { base: f64, height: f64 },

    pub fn area(self: Shape) f64 {
        return switch (self) {
            .circle    => |r| std.math.pi * r * r,
            .rectangle => |rect| rect.w * rect.h,
            .triangle  => |t| 0.5 * t.base * t.height,
        };
    }
};

pub fn main() void {
    const c = Shape{ .circle = 5.0 };
    const r = Shape{ .rectangle = .{ .w = 4, .h = 6 } };
    const t = Shape{ .triangle = .{ .base = 3, .height = 8 } };

    std.debug.print("circle area    = {d:.2}\n", .{c.area()});
    std.debug.print("rectangle area = {d:.2}\n", .{r.area()});
    std.debug.print("triangle area  = {d:.2}\n", .{t.area()});
}
```

---

## 11. 에러 처리

```zig name=error_handling.zig
const std = @import("std");

// 에러 집합 정의
const FileError = error{
    NotFound,
    PermissionDenied,
    InvalidFormat,
};

const MathError = error{
    DivisionByZero,
    Overflow,
};

// 여러 에러 집합 합치기
const AppError = FileError || MathError;

fn riskyOp(x: i32) AppError!i32 {
    if (x < 0) return AppError.DivisionByZero;
    if (x > 100) return AppError.Overflow;
    return x * 2;
}

pub fn main() !void {
    // try: 에러면 즉시 caller에게 전파
    const v1 = try riskyOp(10);
    std.debug.print("v1 = {}\n", .{v1});

    // catch: 에러를 직접 처리
    const v2 = riskyOp(-1) catch |err| blk: {
        std.debug.print("caught: {}\n", .{err});
        break :blk 0;
    };
    std.debug.print("v2 = {}\n", .{v2});

    // catch with default value
    const v3 = riskyOp(200) catch 0;
    std.debug.print("v3 = {}\n", .{v3});

    // errdefer: 에러 발생 시에만 실행되는 defer
    {
        var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
        errdefer arena.deinit(); // 에러 시에만 해제
        defer arena.deinit();    // 항상 해제
        _ = arena;
    }
}
```

---

## 12. 옵셔널 (Optional)

```zig name=optionals.zig
const std = @import("std");

fn findIndex(slice: []const i32, target: i32) ?usize {
    for (slice, 0..) |val, i| {
        if (val == target) return i;
    }
    return null;
}

pub fn main() void {
    const nums = [_]i32{ 10, 20, 30, 40 };

    // if let 패턴
    if (findIndex(&nums, 30)) |idx| {
        std.debug.print("found at {}\n", .{idx}); // 2
    } else {
        std.debug.print("not found\n", .{});
    }

    // orelse: null이면 기본값
    const idx = findIndex(&nums, 99) orelse 999;
    std.debug.print("idx = {}\n", .{idx}); // 999

    // .?: null이면 에러 반환 (unwrap)
    // const idx2 = findIndex(&nums, 20).?; // null이면 패닉

    // while + optional (iterator 패턴)
    var opt: ?i32 = 5;
    while (opt) |val| {
        std.debug.print("opt = {}\n", .{val});
        opt = if (val > 0) val - 1 else null;
    }
}
```

---

## 13. 메모리 관리 & Allocator

```zig name=memory.zig
const std = @import("std");

pub fn main() !void {
    // 1) GeneralPurposeAllocator (디버그용, 누수 감지)
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const allocator = gpa.allocator();

    // 단일 값 할당
    const ptr = try allocator.create(i32);
    defer allocator.destroy(ptr);
    ptr.* = 42;
    std.debug.print("ptr = {}\n", .{ptr.*});

    // 슬라이스 할당
    const buf = try allocator.alloc(u8, 100);
    defer allocator.free(buf);
    buf[0] = 'Z';
    std.debug.print("buf[0] = {c}\n", .{buf[0]});

    // realloc
    var dyn = try allocator.alloc(i32, 5);
    defer allocator.free(dyn);
    dyn = try allocator.realloc(dyn, 10);
    std.debug.print("dyn len = {}\n", .{dyn.len});

    // 2) ArenaAllocator: 한꺼번에 해제 (빠름)
    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer arena.deinit();
    const arena_alloc = arena.allocator();

    const s = try arena_alloc.alloc(u8, 256);
    _ = s;
    // arena.deinit() 시 모두 해제됨

    // 3) FixedBufferAllocator: 스택 버퍼 사용
    var stack_buf: [1024]u8 = undefined;
    var fba = std.heap.FixedBufferAllocator.init(&stack_buf);
    const fba_alloc = fba.allocator();

    const small = try fba_alloc.alloc(u8, 64);
    small[0] = 'A';
    std.debug.print("small[0] = {c}\n", .{small[0]});
}
```

---

## 14. ArrayList & HashMap (std.ArrayList, std.AutoHashMap)

```zig name=collections.zig
const std = @import("std");

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const alloc = gpa.allocator();

    // === ArrayList ===
    var list = std.ArrayList(i32).init(alloc);
    defer list.deinit();

    try list.append(10);
    try list.append(20);
    try list.append(30);
    try list.appendSlice(&[_]i32{ 40, 50 });

    std.debug.print("list len = {}\n", .{list.items.len});
    for (list.items) |v| {
        std.debug.print("{} ", .{v});
    }
    std.debug.print("\n", .{});

    _ = list.pop(); // 마지막 제거
    try list.insert(1, 99); // 인덱스 1에 삽입
    list.swapRemove(0); // 빠른 제거 (순서 유지 안 함)

    // === HashMap ===
    var map = std.AutoHashMap([]const u8, i32).init(alloc);
    defer map.deinit();

    // StringHashMap이 문자열 키에 더 편함
    var smap = std.StringHashMap(i32).init(alloc);
    defer smap.deinit();

    try smap.put("alice", 100);
    try smap.put("bob",   200);
    try smap.put("carol", 300);

    if (smap.get("bob")) |v| {
        std.debug.print("bob = {}\n", .{v}); // 200
    }

    // 순회
    var it = smap.iterator();
    while (it.next()) |entry| {
        std.debug.print("{s} -> {}\n", .{ entry.key_ptr.*, entry.value_ptr.* });
    }

    // remove
    _ = smap.remove("alice");
    std.debug.print("count = {}\n", .{smap.count()});
}
```

---

## 15. 문자열 처리 (std.fmt, std.mem, std.ascii)

```zig name=strings.zig
const std = @import("std");

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const alloc = gpa.allocator();

    // 문자열 포매팅
    const s = try std.fmt.allocPrint(alloc, "Hello, {s}! You are {} years old.", .{ "Zig", 5 });
    defer alloc.free(s);
    std.debug.print("{s}\n", .{s});

    // 숫자 → 문자열
    var nbuf: [32]u8 = undefined;
    const ns = try std.fmt.bufPrint(&nbuf, "{d}", .{12345});
    std.debug.print("num str: {s}\n", .{ns});

    // 문자열 → 숫자
    const num = try std.fmt.parseInt(i32, "9876", 10);
    std.debug.print("parsed: {}\n", .{num});

    const fnum = try std.fmt.parseFloat(f64, "3.14");
    std.debug.print("float: {}\n", .{fnum});

    // std.mem 유틸
    const a = "Hello, World!";
    std.debug.print("startsWith: {}\n", .{std.mem.startsWith(u8, a, "Hello")});
    std.debug.print("endsWith:   {}\n", .{std.mem.endsWith(u8, a, "World!")});
    std.debug.print("contains:   {}\n", .{std.mem.indexOf(u8, a, "World") != null});

    // 비교
    std.debug.print("eql: {}\n", .{std.mem.eql(u8, "abc", "abc")});

    // 복사
    var dst: [5]u8 = undefined;
    @memcpy(&dst, "Hello");
    std.debug.print("copied: {s}\n", .{dst});

    // 분리 (tokenize)
    const csv = "one,two,three,four";
    var tok = std.mem.tokenizeScalar(u8, csv, ',');
    while (tok.next()) |part| {
        std.debug.print("part: {s}\n", .{part});
    }

    // std.ascii
    std.debug.print("upper: {c}\n", .{std.ascii.toUpper('a')});
    std.debug.print("isDigit: {}\n", .{std.ascii.isDigit('9')});
}
```

---

## 16. 파일 I/O (std.fs)

```zig name=file_io.zig
const std = @import("std");

pub fn main() !void {
    // 파일 쓰기
    const file = try std.fs.cwd().createFile("test.txt", .{});
    defer file.close();

    try file.writeAll("Hello, File!\n");
    try file.writer().print("Line {}\n", .{2});

    // 파일 읽기
    const rfile = try std.fs.cwd().openFile("test.txt", .{});
    defer rfile.close();

    var buf: [256]u8 = undefined;
    const bytes_read = try rfile.read(&buf);
    std.debug.print("read {} bytes:\n{s}", .{ bytes_read, buf[0..bytes_read] });

    // 전체 파일 읽기 (allocator 필요)
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const alloc = gpa.allocator();

    const contents = try std.fs.cwd().readFileAlloc(alloc, "test.txt", 1024 * 1024);
    defer alloc.free(contents);
    std.debug.print("full content:\n{s}", .{contents});

    // 디렉토리 생성
    std.fs.cwd().makeDir("test_dir") catch |err| {
        if (err != error.PathAlreadyExists) return err;
    };

    // 파일 삭제
    try std.fs.cwd().deleteFile("test.txt");
    try std.fs.cwd().deleteDir("test_dir");
}
```

---

## 17. 표준 입력 (stdin)

```zig name=stdin.zig
const std = @import("std");

pub fn main() !void {
    const stdin = std.io.getStdIn().reader();
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const alloc = gpa.allocator();

    std.debug.print("Enter your name: ", .{});

    // 한 줄 읽기
    const line = try stdin.readUntilDelimiterAlloc(alloc, '\n', 1024);
    defer alloc.free(line);

    // Windows 개행(\r\n) 처리
    const name = std.mem.trimRight(u8, line, "\r\n");
    std.debug.print("Hello, {s}!\n", .{name});
}
```

---

## 18. 멀티스레딩 (std.Thread)

```zig name=threading.zig
const std = @import("std");

fn worker(id: usize) void {
    std.debug.print("Thread {} running\n", .{id});
    std.time.sleep(100 * std.time.ns_per_ms);
    std.debug.print("Thread {} done\n", .{id});
}

pub fn main() !void {
    const thread_count = 4;
    var threads: [thread_count]std.Thread = undefined;

    for (0..thread_count) |i| {
        threads[i] = try std.Thread.spawn(.{}, worker, .{i});
    }
    for (&threads) |*t| {
        t.join();
    }

    std.debug.print("All threads finished\n", .{});
}
```

---

## 19. Mutex & Atomic

```zig name=mutex_atomic.zig
const std = @import("std");

var counter: i64 = 0;
var mutex = std.Thread.Mutex{};

fn increment(n: usize) void {
    for (0..n) |_| {
        mutex.lock();
        defer mutex.unlock();
        counter += 1;
    }
}

// Atomic 사용
var atomic_counter = std.atomic.Value(i64).init(0);

fn atomicIncrement(n: usize) void {
    for (0..n) |_| {
        _ = atomic_counter.fetchAdd(1, .monotonic);
    }
}

pub fn main() !void {
    var t1 = try std.Thread.spawn(.{}, increment, .{1000});
    var t2 = try std.Thread.spawn(.{}, increment, .{1000});
    t1.join();
    t2.join();
    std.debug.print("mutex counter = {}\n", .{counter}); // 2000

    var t3 = try std.Thread.spawn(.{}, atomicIncrement, .{1000});
    var t4 = try std.Thread.spawn(.{}, atomicIncrement, .{1000});
    t3.join();
    t4.join();
    std.debug.print("atomic counter = {}\n", .{atomic_counter.load(.monotonic)}); // 2000
}
```

---

## 20. comptime (컴파일 타임 프로그래밍)

```zig name=comptime.zig
const std = @import("std");

// comptime 제네릭 함수
fn Stack(comptime T: type) type {
    return struct {
        items: std.ArrayList(T),

        pub fn init(alloc: std.mem.Allocator) @This() {
            return .{ .items = std.ArrayList(T).init(alloc) };
        }
        pub fn push(self: *@This(), val: T) !void {
            try self.items.append(val);
        }
        pub fn pop(self: *@This()) ?T {
            return self.items.popOrNull();
        }
        pub fn deinit(self: *@This()) void {
            self.items.deinit();
        }
    };
}

// comptime 조건부 코드
fn typeInfo(comptime T: type) void {
    const info = @typeInfo(T);
    switch (info) {
        .int    => std.debug.print("{} is an int\n", .{T}),
        .float  => std.debug.print("{} is a float\n", .{T}),
        .bool   => std.debug.print("{} is a bool\n", .{T}),
        .@"struct" => std.debug.print("{} is a struct\n", .{T}),
        else    => std.debug.print("{} is something else\n", .{T}),
    }
}

// 컴파일 타임 계산
fn fibonacci(comptime n: u64) u64 {
    if (n <= 1) return n;
    return comptime fibonacci(n - 1) + fibonacci(n - 2);
}

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();

    var stack = Stack(i32).init(gpa.allocator());
    defer stack.deinit();

    try stack.push(1);
    try stack.push(2);
    try stack.push(3);
    std.debug.print("pop = {?}\n", .{stack.pop()});

    typeInfo(i32);
    typeInfo(f64);
    typeInfo(bool);

    const fib10 = comptime fibonacci(10);
    std.debug.print("fib(10) = {}\n", .{fib10}); // 55
}
```

---

## 21. 빌트인 함수 (@builtins)

```zig name=builtins.zig
const std = @import("std");

pub fn main() void {
    // 타입 변환
    const a: i32 = 100;
    const b: f64 = @floatFromInt(a);
    const c: i32 = @intFromFloat(b);
    const d: u8  = @intCast(a);          // 오버플로우 검사 포함
    const e: i64 = @as(i64, a);          // 타입 명시 캐스팅

    // 비트 연산
    const bits: u8 = 0b1010_1010;
    std.debug.print("popcount  = {}\n", .{@popCount(bits)});   // 1인 비트 수
    std.debug.print("clz       = {}\n", .{@clz(bits)});        // 앞 0 개수
    std.debug.print("ctz       = {}\n", .{@ctz(bits)});        // 뒤 0 개수
    std.debug.print("bitReverse= {b}\n", .{@bitReverse(bits)});
    std.debug.print("byteSwap  = {}\n", .{@byteSwap(@as(u16, 0x1234))});

    // 산술 (오버플로우 체크)
    const res = @addWithOverflow(@as(u8, 200), @as(u8, 100));
    std.debug.print("overflow  = {} flag={}\n", .{ res[0], res[1] });

    // 타입 정보
    std.debug.print("sizeof i32  = {}\n", .{@sizeOf(i32)});
    std.debug.print("alignof f64 = {}\n", .{@alignOf(f64)});
    std.debug.print("bitsize u7  = {}\n", .{@bitSizeOf(u7)});

    // 타입 이름
    std.debug.print("type name  = {s}\n", .{@typeName(f32)});

    _ = b; _ = c; _ = d; _ = e;
}
```

---

## 22. 테스트 (std.testing)

```zig name=tests.zig
const std = @import("std");

fn add(a: i32, b: i32) i32 {
    return a + b;
}

fn divide(a: i32, b: i32) !i32 {
    if (b == 0) return error.DivisionByZero;
    return @divTrunc(a, b);
}

test "basic addition" {
    try std.testing.expect(add(2, 3) == 5);
    try std.testing.expectEqual(@as(i32, 10), add(3, 7));
}

test "division" {
    const result = try divide(10, 2);
    try std.testing.expectEqual(@as(i32, 5), result);
}

test "division by zero returns error" {
    try std.testing.expectError(error.DivisionByZero, divide(5, 0));
}

test "string equality" {
    try std.testing.expectEqualStrings("hello", "hello");
}

test "slice equality" {
    const a = [_]i32{ 1, 2, 3 };
    const b = [_]i32{ 1, 2, 3 };
    try std.testing.expectEqualSlices(i32, &a, &b);
}

// zig test tests.zig 으로 실행
```

---

## 23. 인라인 어셈블리 & C 인터롭

```zig name=c_interop.zig
const std = @import("std");
const c = @cImport({
    @cInclude("stdio.h");
    @cInclude("stdlib.h");
});

pub fn main() void {
    // C 함수 호출
    _ = c.printf("Hello from C printf!\n");

    // C 메모리 할당
    const ptr = c.malloc(16) orelse {
        std.debug.print("malloc failed\n", .{});
        return;
    };
    defer c.free(ptr);

    // 인라인 어셈블리 (x86_64)
    const result = asm volatile ("syscall"
        : [ret] "={rax}" (-> usize),
        : [nr]  "{rax}"  (@as(usize, 39)), // getpid syscall
        : "rcx", "r11"
    );
    std.debug.print("pid = {}\n", .{result});
}
```

---

## 📌 핵심 요약표

| 문법/기능 | 키워드/패턴 |
|---|---|
| 상수/변수 | `const` / `var` |
| 에러 반환 | `!T`, `try`, `catch`, `errdefer` |
| 옵셔널 | `?T`, `orelse`, `.?`, `if (x) \|v\|` |
| 제네릭 | `comptime T: type`, `@This()` |
| 메모리 | `allocator.alloc/free/create/destroy` |
| 컬렉션 | `ArrayList`, `StringHashMap`, `AutoHashMap` |
| 빌트인 | `@sizeOf`, `@typeInfo`, `@intCast`, `@as` 등 |
| 테스트 | `test "..."`, `std.testing.expect*` |
| C 연동 | `@cImport`, `@cInclude` |
| 멀티스레드 | `std.Thread.spawn`, `Mutex`, `atomic.Value` |

---
망할 도마뱀 🦎


# 커맨드

# 🦎 Zig CLI 커맨드 완전 정리

## 1. 설치 & 버전 확인

```bash name=setup.sh
# 버전 확인
zig version

# 환경 정보
zig env

# 도움말
zig help
zig build --help
```

---

## 2. `zig run` — 바로 실행

```bash name=run.sh
# 단일 파일 즉시 컴파일 + 실행
zig run main.zig

# 인자 전달 (-- 이후가 프로그램 인자)
zig run main.zig -- arg1 arg2

# 최적화 모드 지정
zig run main.zig -O Debug        # 기본값, 디버그 정보 포함
zig run main.zig -O ReleaseSafe  # 최적화 + 안전 검사 유지
zig run main.zig -O ReleaseFast  # 최대 최적화 (안전 검사 제거)
zig run main.zig -O ReleaseSmall # 최소 바이너리 크기

# C 라이브러리 링크
zig run main.zig -lc

# 외부 라이브러리/인클루드 경로 지정
zig run main.zig -I/usr/include -L/usr/lib -lssl
```

---

## 3. `zig build-exe` — 단일 파일 컴파일

```bash name=build_exe.sh
# 실행 파일 빌드
zig build-exe main.zig

# 출력 이름 지정
zig build-exe main.zig -femit-bin=myapp

# 최적화 + 출력 이름
zig build-exe main.zig -O ReleaseFast -femit-bin=myapp

# 크로스 컴파일 (타겟 지정)
zig build-exe main.zig -target x86_64-linux-gnu
zig build-exe main.zig -target aarch64-linux-gnu
zig build-exe main.zig -target x86_64-windows-gnu
zig build-exe main.zig -target aarch64-macos

# C 링크 포함
zig build-exe main.zig -lc

# 스트립 (디버그 심볼 제거 → 바이너리 경량화)
zig build-exe main.zig -O ReleaseSmall --strip

# 싱글 스레드 모드
zig build-exe main.zig -fsingle-threaded
```

---

## 4. `zig build-lib` — 라이브러리 컴파일

```bash name=build_lib.sh
# 정적 라이브러리 (.a)
zig build-lib mylib.zig

# 동적 라이브러리 (.so / .dll / .dylib)
zig build-lib mylib.zig -dynamic

# 이름 지정
zig build-lib mylib.zig -femit-bin=libmylib.a

# C ABI 호환 동적 라이브러리
zig build-lib mylib.zig -dynamic -lc -femit-bin=libmylib.so
```

---

## 5. `zig build-obj` — 오브젝트 파일 생성

```bash name=build_obj.sh
# .o 파일 생성 (다른 링커와 연동 시)
zig build-obj main.zig

# LLVM IR 출력
zig build-obj main.zig -femit-llvm-ir

# 어셈블리 출력
zig build-obj main.zig -femit-asm
```

---

## 6. `zig test` — 테스트 실행

```bash name=test.sh
# 파일 내 test 블록 모두 실행
zig test main.zig

# 특정 테스트만 (이름 필터)
zig test main.zig --test-filter "addition"

# 여러 필터
zig test main.zig --test-filter "add" --test-filter "divide"

# 최적화 모드로 테스트
zig test main.zig -O ReleaseSafe

# 테스트 타임아웃 설정 (ms)
zig test main.zig --test-timeout-ms 5000

# 크로스 컴파일 테스트 (QEMU 필요)
zig test main.zig -target aarch64-linux-gnu

# 테스트 바이너리만 빌드 (실행 안 함)
zig test main.zig --test-no-exec
```

---

## 7. `zig fmt` — 코드 포매터

```bash name=fmt.sh
# 단일 파일 포매팅
zig fmt main.zig

# 디렉토리 전체 포매팅
zig fmt src/

# 검사만 (변경 없음, CI에서 유용)
zig fmt --check main.zig
zig fmt --check src/

# stdin → stdout 포매팅 (파이프)
cat main.zig | zig fmt --stdin
```

---

## 8. `zig init` — 프로젝트 초기화

```bash name=init.sh
# 새 프로젝트 디렉토리에서 실행
mkdir myproject && cd myproject

# 프로젝트 초기화 (build.zig + src/main.zig 생성)
zig init

# 생성되는 구조:
# myproject/
# ├── build.zig         ← 빌드 스크립트
# ├── build.zig.zon     ← 패키지 메타데이터 (의존성)
# └── src/
#     ├── main.zig      ← 실행 파일 진입점
#     └── root.zig      ← 라이브러리 진입점
```

---

## 9. `zig build` — 프로젝트 빌드 시스템

```bash name=zig_build.sh
# 기본 빌드 (build.zig 기준)
zig build

# 특정 스텝 실행
zig build run          # 빌드 후 실행
zig build test         # 테스트 실행
zig build install      # zig-out/bin/ 에 설치

# 최���화 모드 지정
zig build -Doptimize=Debug
zig build -Doptimize=ReleaseSafe
zig build -Doptimize=ReleaseFast
zig build -Doptimize=ReleaseSmall

# 타겟 지정 (크로스 컴파일)
zig build -Dtarget=x86_64-linux-gnu
zig build -Dtarget=aarch64-linux-gnu
zig build -Dtarget=x86_64-windows-gnu
zig build -Dtarget=wasm32-freestanding

# 커스텀 옵션 전달 (build.zig에서 정의한 옵션)
zig build -Dfeature=true

# 출력 디렉토리 지정
zig build --prefix ./dist

# 병렬 빌드 (기본값: CPU 코어 수)
zig build -j4

# 빌드 + 즉시 실행 + 인자 전달
zig build run -- --port 8080

# 빌드 캐시 초기화
zig build --summary all
```

---

## 10. `build.zig` 파일 작성법

```zig name=build.zig
const std = @import("std");

pub fn build(b: *std.Build) void {
    // 타겟 & 최적화 옵션 (zig build -Dtarget=... -Doptimize=... 로 오버라이드 가능)
    const target   = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    // ── 실행 파일 ──────────────────────────────────────────
    const exe = b.addExecutable(.{
        .name     = "myapp",
        .root_source_file = b.path("src/main.zig"),
        .target   = target,
        .optimize = optimize,
    });

    // C 라이브러리 링크
    exe.linkLibC();
    exe.linkSystemLibrary("ssl");

    // 인클루드 경로 추가
    exe.addIncludePath(b.path("include"));

    // C 소스 파일 추가
    exe.addCSourceFile(.{
        .file  = b.path("src/helper.c"),
        .flags = &[_][]const u8{"-std=c11"},
    });

    // 설치 스텝 등록 (zig-out/bin/ 에 복사)
    b.installArtifact(exe);

    // ── run 스텝 ───────────────────────────────────────────
    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| run_cmd.addArgs(args);

    const run_step = b.step("run", "Run the application");
    run_step.dependOn(&run_cmd.step);

    // ── 테스트 스텝 ────────────────────────────────────────
    const unit_tests = b.addTest(.{
        .root_source_file = b.path("src/main.zig"),
        .target   = target,
        .optimize = optimize,
    });
    const run_unit_tests = b.addRunArtifact(unit_tests);
    const test_step = b.step("test", "Run unit tests");
    test_step.dependOn(&run_unit_tests.step);

    // ── 라이브러리 ─────────────────────────────────────────
    const lib = b.addStaticLibrary(.{
        .name     = "mylib",
        .root_source_file = b.path("src/root.zig"),
        .target   = target,
        .optimize = optimize,
    });
    b.installArtifact(lib);

    // ── 커스텀 옵션 ────────────────────────────────────────
    const enable_feature = b.option(bool, "feature", "Enable feature X") orelse false;
    const opts = b.addOptions();
    opts.addOption(bool, "enable_feature", enable_feature);
    exe.root_module.addOptions("config", opts);
}
```

---

## 11. `build.zig.zon` — 패키지 & 의존성 관리

```zig name=build.zig.zon
.{
    .name    = "myapp",
    .version = "0.1.0",
    .minimum_zig_version = "0.13.0",

    .dependencies = .{
        // URL 기반 의존성
        .zap = .{
            .url  = "https://github.com/zigzap/zap/archive/refs/tags/v0.9.0.tar.gz",
            .hash = "1220abc123...",  // zig fetch 로 자동 생성
        },
        // 로컬 경로 의존성
        .mylib = .{
            .path = "../mylib",
        },
    },

    .paths = .{
        "src",
        "build.zig",
        "build.zig.zon",
        "README.md",
    },
}
```

```bash name=package.sh
# 의존성 해시 자동 계산 (hash 값 채워줌)
zig fetch --save https://github.com/zigzap/zap/archive/refs/tags/v0.9.0.tar.gz

# 로컬 경로 패키지 추가
zig fetch --save path=../mylib
```

---

## 12. 크로스 컴파일 & 지원 타겟

```bash name=cross_compile.sh
# 지원 가능한 모든 타겟 목록
zig targets

# 자주 쓰는 타겟들
zig build -Dtarget=x86_64-linux-gnu        # Linux x86_64 (glibc)
zig build -Dtarget=x86_64-linux-musl       # Linux x86_64 (musl, 정적 링크)
zig build -Dtarget=aarch64-linux-gnu       # Linux ARM64
zig build -Dtarget=x86_64-windows-gnu      # Windows x86_64
zig build -Dtarget=aarch64-macos           # macOS Apple Silicon
zig build -Dtarget=x86_64-macos            # macOS Intel
zig build -Dtarget=wasm32-freestanding     # WebAssembly (OS 없음)
zig build -Dtarget=wasm32-wasi             # WebAssembly (WASI)
zig build -Dtarget=arm-linux-gnueabihf     # Raspberry Pi (32bit)
zig build -Dtarget=riscv64-linux-gnu       # RISC-V 64bit

# 배포용 정적 바이너리 (Linux musl + strip)
zig build -Dtarget=x86_64-linux-musl -Doptimize=ReleaseSmall
```

---

## 13. C/C++ 컴파일러로 사용 (Zig as CC)

```bash name=zig_as_cc.sh
# Zig를 C 컴파일러로 사용
zig cc main.c -o main

# Zig를 C++ 컴파일러로 사용
zig c++ main.cpp -o main

# 크로스 컴파일 C 코드
zig cc main.c -target x86_64-linux-musl -o main-linux

# C 코드를 다른 플랫폼용으로 크로스 컴파일
zig cc main.c -target x86_64-windows-gnu -o main.exe

# CMAKE / Makefile 프로젝트에서 Zig CC 사용
CC="zig cc" CXX="zig c++" cmake ..
CC="zig cc" make

# ar (라이브러리 아카이버) 대체
zig ar rcs libfoo.a foo.o bar.o
```

---

## 14. `zig translate-c` — C 헤더 변환

```bash name=translate_c.sh
# C 헤더를 Zig 코드로 변환 (바인딩 생성 참고용)
zig translate-c myheader.h

# 시스템 헤더 변환
zig translate-c /usr/include/stdio.h

# 출력을 파일로 저장
zig translate-c myheader.h > bindings.zig

# 매크로 정의 포함
zig translate-c -DDEBUG=1 myheader.h
```

---

## 15. `zig ast-check` & `zig reduce` — 디버깅 도구

```bash name=debug_tools.sh
# AST 구문 검사 (컴파일 없이 문법 오류 확인)
zig ast-check main.zig
zig ast-check src/

# 크래시 재현 코드 최소화 (컴파일러 버그 리포트용)
zig reduce main.zig

# 오브젝트 파일 심볼 확인
zig objcopy --help
```

---

## 16. 자주 쓰는 전체 워크플로우

```bash name=workflow.sh
# ── 개발 중 ───────────────────────────────────────��─────
zig init                          # 프로젝트 초기화
zig build run                     # 빌드 + 실행
zig build run -- --port 8080      # 빌드 + 실행 + 인자
zig build test                    # 테스트
zig fmt src/                      # 코드 포매팅

# ── CI ──────────────────────────────────────────────────
zig fmt --check src/              # 포매팅 검사
zig build test -Doptimize=ReleaseSafe  # 안전 모드 테스트
zig build -Dtarget=x86_64-linux-musl  # 크로스 컴파일 검증

# ── 배포 ────────────────────────────────────────────────
# Linux 정적 바이너리 (가장 이식성 높음)
zig build -Dtarget=x86_64-linux-musl -Doptimize=ReleaseSmall
# → zig-out/bin/myapp

# Windows 바이너리
zig build -Dtarget=x86_64-windows-gnu -Doptimize=ReleaseFast
# → zig-out/bin/myapp.exe

# macOS (Apple Silicon)
zig build -Dtarget=aarch64-macos -Doptimize=ReleaseFast
# → zig-out/bin/myapp

# 바이너리 크기 확인
ls -lh zig-out/bin/myapp
strip zig-out/bin/myapp           # 추가 strip (선택)

# 모든 플랫폼 한번에 빌드 (shell script)
for TARGET in x86_64-linux-musl x86_64-windows-gnu aarch64-macos; do
    zig build -Dtarget=$TARGET -Doptimize=ReleaseSmall
    echo "Built for $TARGET"
done
```

---

## 📌 최적화 모드 요약

| 모드 | 설명 | 용도 |
|---|---|---|
| `Debug` | 안전 검사 최대, 최적화 없음 | 개발 중 기본값 |
| `ReleaseSafe` | 최적화 + 안전 검사 유지 | 프로덕션 (안전 우선) |
| `ReleaseFast` | 최대 최적화, 안전 검사 제거 | 프로덕션 (성능 우선) |
| `ReleaseSmall` | 바이너리 크기 최소화 | 임베디드, 배포 크기 최소화 |

## 📌 타겟 링커 요약

| 타겟 접미사 | 설명 |
|---|---|
| `-gnu` | glibc 동적 링크 (시스템 glibc 필요) |
| `-musl` | musl 정적 링크 (독립 실행, 이식성 최고) |
| `-none` | OS 없음 (임베디드, bare metal) |
| `wasm32-freestanding` | WebAssembly, OS 없음 |
| `wasm32-wasi` | WebAssembly, WASI 시스템 콜 |

---

> 💡 **배포 시 가장 추천하는 조합**: `-Dtarget=x86_64-linux-musl -Doptimize=ReleaseSmall`
> → 외부 의존성 없는 단일 정적 바이너리, 어떤 Linux에서도 실행 가능


copilot hell