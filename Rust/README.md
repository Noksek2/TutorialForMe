# Rust 사용법 및 문법 가이드

## 목차
1. [Rust 소개](#rust-소개)
2. [설치 및 환경 설정](#설치-및-환경-설정)
3. [기본 문법](#기본-문법)
4. [변수 및 데이터 타입](#변수-및-데이터-타입)
5. [함수](#함수)
6. [제어 흐름](#제어-흐름)
7. [소유권 시스템](#소유권-시스템)
8. [참조와 빌림](#참조와-빌림)
9. [구조체](#구조체)
10. [열거형](#열거형)
11. [패턴 매칭](#패턴-매칭)
12. [에러 처리](#에러-처리)
13. [컬렉션](#컬렉션)
14. [제네릭](#제네릭)
15.  [트레이트](#트레이트)
16. [모듈 및 크레이트](#모듈-및-크레이트)

---

## Rust 소개

Rust는 메모리 안전성과 성능을 모두 제공하는 현대적인 시스템 프로그래밍 언어입니다. 

### 주요 특징
- **메모리 안전성**: 컴파일 타임에 메모리 안전성 보장
- **높은 성능**: C/C++와 비슷한 성능
- **동시성**: 안전한 멀티스레딩
- **표현력**: 함수형과 명령형 패러다임 모두 지원

---

## 설치 및 환경 설정

### macOS/Linux
```bash
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
```

### Windows
[rustup-init.exe](https://rustup.rs/) 다운로드 후 실행

### 버전 확인
```bash
rustc --version
cargo --version
```

### 새 프로젝트 생성
```bash
cargo new my_project
cd my_project
```

### 프로젝트 구조
```
my_project/
├── Cargo.toml      # 프로젝트 메타데이터 및 의존성
└── src/
    └── main.rs     # 메인 소스 코드
```

---

## 기본 문법

### Hello, World! 

```rust
fn main() {
    println!("Hello, world!");
}
```

### 매크로 vs 함수
- `println!()` - 매크로 (끝에 `!`)
- `fn()` - 함수

---

## 변수 및 데이터 타입

### 변수 선언

#### 불변 변수 (기본값)
```rust
let x = 5;  // i32로 추론
let x: i32 = 5;  // 명시적 타입
```

#### 가변 변수
```rust
let mut y = 5;
y = 6;  // 가능
```

#### 상수
```rust
const MAX_POINTS: u32 = 100_000;
```

### 스코프와 섀도잉
```rust
let x = 5;
let x = x + 1;  // 섀도잉: 새로운 변수로 덮음
{
    let x = x * 2;
    println!("{}", x);  // 12
}
println!("{}", x);  // 6
```

### 기본 데이터 타입

#### 정수형
```rust
let a: i8 = -5;        // 8비트 부호 있음
let b: u16 = 100;      // 16비트 부호 없음
let c: i32 = -2_000;   // 32비트 (기본값)
let d: i64 = 9_000_000_000;  // 64비트
let e: usize = 10;     // 아키텍처 의존 (포인터 크기)
```

#### 부동 소수점
```rust
let x = 2.0;           // f64 (기본값)
let y: f32 = 3.14;     // f32
```

#### 불린
```rust
let t = true;
let f: bool = false;
```

#### 문자와 문자열
```rust
let c = 'z';           // char (유니코드 스칼라 값)
let s = "hello";       // &str (문자열 슬라이스)
let s = String::from("hello");  // String (힙 할당)
```

### 튜플
```rust
let tup: (i32, f64, u8) = (500, 6.4, 1);
let (x, y, z) = tup;   // 분해
let first = tup.0;     // 인덱싱
```

### 배열
```rust
let a = [1, 2, 3, 4, 5];
let a: [i32; 5] = [1, 2, 3, 4, 5];  // 명시적 타입과 길이
let a = [3; 5];        // [3, 3, 3, 3, 3]
let first = a[0];
```

---

## 함수

### 함수 선언
```rust
fn add_one(x: i32) -> i32 {
    x + 1  // 반환값 (세미콜론 없음)
}

fn print_number(x: i32) {
    println!("{}", x);  // 반환값 없음
}
```

### 표현식 vs 문장
```rust
let x = 5 + 6;         // 표현식, 값을 반환
let y = {
    let x = 3;
    x + 1              // 표현식, 4 반환
};                     // 세미콜론 있으면 값이 없음

fn example() {
    let x = 5;         // 문장
    let y = (let z = 6);  // 오류!  let은 값을 반환하지 않음
}
```

---

## 제어 흐름

### if 표현식
```rust
let number = 6;

if number % 4 == 0 {
    println!("divisible by 4");
} else if number % 3 == 0 {
    println! ("divisible by 3");
} else {
    println!("not divisible");
}

// if는 표현식
let condition = true;
let number = if condition { 5 } else { 6 };
```

### loop (무한 루프)
```rust
let mut count = 0;
loop {
    count += 1;
    if count == 3 {
        break;  // 루프 종료
    }
    println!("{}", count);
}

// 루프 레이블
'outer: loop {
    loop {
        break 'outer;  // 외부 루프 종료
    }
}

// 루프에서 값 반환
let result = loop {
    count += 1;
    if count == 10 {
        break count * 2;  // 20 반환
    }
};
```

### while 루프
```rust
let mut number = 3;
while number != 0 {
    println!("{}", number);
    number -= 1;
}
println!("LIFTOFF!! !");
```

### for 루프
```rust
let a = [10, 20, 30, 40, 50];

// 인덱스로 반복
for i in 0.. 5 {
    println!("{}", a[i]);
}

// 범위로 반복
for i in 1..4 {
    println!("{}", i);  // 1, 2, 3
}

// 범위 포함 (inclusive)
for i in 1..=4 {
    println!("{}", i);  // 1, 2, 3, 4
}

// 컬렉션 요소로 반복
for element in &a {
    println!("{}", element);
}

// 역순
for i in (1..4).rev() {
    println! ("{}", i);  // 3, 2, 1
}
```

---

## 소유권 시스템

Rust의 핵심: **메모리 안전성**을 소유권으로 보장합니다.

### 소유권 규칙
1.  Rust의 각 값은 소유자라는 변수를 가짐
2. 한 번에 하나의 소유자만 가능
3. 소유자가 스코프를 벗어나면 값이 드롭됨

### 예제
```rust
let s1 = String::from("hello");
let s2 = s1;  // s1은 더 이상 유효하지 않음 (이동/Move)

// println!("{}", s1);  // 오류! 
println!("{}", s2);    // "hello"

// 깊은 복사 (clone)
let s1 = String::from("hello");
let s2 = s1.clone();
println!("{}, {}", s1, s2);  // 둘 다 유효
```

### 스택 vs 힙
```rust
// 스택: 고정 크기, 복사됨
let x = 5;
let y = x;
println!("{}, {}", x, y);  // x는 여전히 유효

// 힙: 가변 크기, 이동됨
let s1 = String::from("hello");
let s2 = s1;  // s1은 유효하지 않음
```

### 함수와 소유권
```rust
fn takes_ownership(s: String) {
    println!("{}", s);
}  // s가 스코프를 벗어나고 drop됨

fn makes_copy(x: i32) {
    println!("{}", x);
}  // x는 i32이므로 복사됨, x가 복사본이므로 drop됨

let s = String::from("hello");
takes_ownership(s);
// println!("{}", s);  // 오류! s는 이동됨

let x = 5;
makes_copy(x);
println!("{}", x);  // 5 (여전히 유효)
```

### 값 반환과 소유권
```rust
fn gives_ownership() -> String {
    let s = String::from("hello");
    s  // s가 호출자에게 반환됨
}

fn takes_and_gives_back(s: String) -> String {
    s  // s가 반환됨
}

let s1 = gives_ownership();
let s2 = String::from("hello");
let s3 = takes_and_gives_back(s2);  // s2는 이동됨
```

---

## 참조와 빌림

### 참조 (&)
```rust
fn calculate_length(s: &String) -> usize {
    s.len()
}  // s는 스코프를 벗어나지만, 소유하지 않았으므로 drop되지 않음

let s1 = String::from("hello");
let len = calculate_length(&s1);  // 참조 전달
println!("'{}' has length {}", s1, len);  // s1은 유효
```

### 가변 참조 (&mut)
```rust
fn change(s: &mut String) {
    s.push_str(", world");
}

let mut s = String::from("hello");
change(&mut s);
println!("{}", s);  // "hello, world"
```

### 빌림 규칙
1. **한 번에 하나의 가변 참조만 가능**
```rust
let mut s = String::from("hello");

let r1 = &mut s;
let r2 = &mut s;  // 오류! r1이 여전히 스코프 내에 있음

println!("{}, {}", r1, r2);
```

2. **불변 참조가 있을 때 가변 참조는 불가능**
```rust
let mut s = String::from("hello");

let r1 = &s;
let r2 = &s;
let r3 = &mut s;  // 오류! 

println!("{}, {}, {}", r1, r2, r3);
```

3. **불변 참조 여러 개는 가능**
```rust
let s = String::from("hello");

let r1 = &s;
let r2 = &s;
println!("{}, {}", r1, r2);  // OK
```

---

## 구조체

### 기본 구조체
```rust
struct User {
    username: String,
    email: String,
    sign_in_count: u64,
    active: bool,
}

// 인스턴스 생성
let mut user1 = User {
    username: String::from("someone"),
    email: String::from("someone@example.com"),
    sign_in_count: 1,
    active: true,
};

user1.email = String::from("another@example.com");

// 필드 초기화 축약
let username = String::from("someusername123");
let user = User {
    username,  // username: username,
    email: String::from("someone@example.com"),
    sign_in_count: 1,
    active: true,
};

// 구조체 업데이트 문법
let user2 = User {
    username: String::from("user2"),
    .. user1  // user1의 나머지 필드 복사
};
```

### 튜플 구조체
```rust
struct Color(i32, i32, i32);
struct Point(i32, i32, i32);

let black = Color(0, 0, 0);
let origin = Point(0, 0, 0);

let Color(r, g, b) = black;
```

### 단위 구조체
```rust
struct AlwaysEqual;

let subject = AlwaysEqual;
```

### 메서드
```rust
#[derive(Debug)]
struct Rectangle {
    width: u32,
    height: u32,
}

impl Rectangle {
    fn area(&self) -> u32 {
        self.width * self. height
    }

    fn can_hold(&self, other: &Rectangle) -> bool {
        self.width > other.width && self.height > other.height
    }

    // 연관 함수 (self 파라미터 없음)
    fn square(size: u32) -> Rectangle {
        Rectangle {
            width: size,
            height: size,
        }
    }
}

let rect = Rectangle {
    width: 30,
    height: 50,
};

println!("Area: {}", rect.area());

let square = Rectangle::square(25);
```

---

## 열거형

### 기본 열거형
```rust
enum IpAddrKind {
    V4,
    V6,
}

struct IpAddr {
    kind: IpAddrKind,
    address: String,
}

let home = IpAddr {
    kind: IpAddrKind::V4,
    address: String::from("127.0.0.1"),
};
```

### 열거형에 데이터 연결
```rust
enum IpAddr {
    V4(String),
    V6(String),
}

let home = IpAddr::V4(String::from("127.0. 0.1"));
let loopback = IpAddr::V6(String::from("::1"));

// 다양한 타입
enum Message {
    Quit,
    Move { x: i32, y: i32 },
    Write(String),
    ChangeColor(i32, i32, i32),
}

let m = Message::Write(String::from("hello"));
```

### 열거형 메서드
```rust
impl Message {
    fn call(&self) {
        // 메서드 정의
    }
}

let m = Message::Write(String::from("hello"));
m.call();
```

### Option 열거형
```rust
enum Option<T> {
    Some(T),
    None,
}

// Rust에서 Option은 기본 제공
let some_number = Some(5);
let some_string = Some("a string");
let absent_number: Option<i32> = None;

// Option 사용
let x: i32 = 5;
let y: Option<i32> = Some(5);

// let sum = x + y;  // 오류! i32와 Option<i32>는 더할 수 없음
```

### Result 열거형
```rust
enum Result<T, E> {
    Ok(T),
    Err(E),
}
```

---

## 패턴 매칭

### match 표현식
```rust
#[derive(Debug)]
enum UsState {
    Alabama,
    Alaska,
}

enum Coin {
    Penny,
    Nickel,
    Dime,
    Quarter(UsState),
}

fn value_in_cents(coin: Coin) -> u8 {
    match coin {
        Coin::Penny => 1,
        Coin::Nickel => 5,
        Coin::Dime => 10,
        Coin::Quarter(state) => {
            println!("State quarter from {:?}!", state);
            25
        }
    }
}
```

### Option 매칭
```rust
fn plus_one(x: Option<i32>) -> Option<i32> {
    match x {
        None => None,
        Some(i) => Some(i + 1),
    }
}

let five = Some(5);
let six = plus_one(five);
let none = plus_one(None);
```

### 캐치올 패턴 (_)
```rust
let some_value = 0u8;

match some_value {
    1 => println!("one"),
    3 => println!("three"),
    5 => println!("five"),
    7 => println!("seven"),
    _ => println!("anything else"),
}

// 나머지를 변수로 바인딩
match some_value {
    1 => println!("one"),
    other => println!("other value: {}", other),
}
```

### if let (간결한 문법)
```rust
let some_option = Some(3);

// match 대신 if let 사용
if let Some(3) = some_option {
    println!("three");
} else {
    println!("not three");
}

// 위의 match와 동일
match some_option {
    Some(3) => println!("three"),
    _ => println!("not three"),
}
```

---

## 에러 처리

### panic!  매크로 (복구 불가)
```rust
fn main() {
    // panic!("crash and burn");
    let v = vec![1, 2, 3];
    v[99];  // panic!
}
```

### Result 타입 (복구 가능)
```rust
use std::fs::File;
use std::io::ErrorKind;

fn main() {
    let f = File::open("hello.txt");

    // match로 처리
    let f = match f {
        Ok(file) => file,
        Err(error) => match error.kind() {
            ErrorKind::NotFound => match File::create("hello.txt") {
                Ok(fc) => fc,
                Err(e) => panic!("Problem creating file: {:?}", e),
            },
            other_error => panic!("Problem opening file: {:?}", other_error),
        },
    };
}
```

### 편의 메서드
```rust
use std::fs::File;

// unwrap: Ok면 값 반환, Err면 panic! 
let f = File::open("hello.txt"). unwrap();

// expect: 커스텀 메시지와 함께 panic! 
let f = File::open("hello.txt")
    .expect("Failed to open hello.txt");

// unwrap_or: 기본값 설정
let f = File::open("hello.txt"). unwrap_or_else(|error| {
    if error.kind() == std::io::ErrorKind::NotFound {
        File::create("hello.txt").unwrap_or_else(|error| {
            panic!("Problem creating file: {:?}", error);
        })
    } else {
        panic!("Problem opening file: {:?}", error);
    }
});
```

### ?  연산자 (에러 전파)
```rust
use std::fs::File;
use std::io::{self, Read};

fn read_username_from_file() -> Result<String, io::Error> {
    let mut f = File::open("hello.txt")?;  // Err면 반환
    let mut s = String::new();
    f.read_to_string(&mut s)?;
    Ok(s)
}

// 더 간단히
fn read_username_from_file() -> Result<String, io::Error> {
    let mut s = String::new();
    File::open("hello.txt")?. read_to_string(&mut s)?;
    Ok(s)
}
```

---

## 컬렉션

### Vector
```rust
// 생성
let mut v: Vec<i32> = Vec::new();
v.push(5);
v.push(6);

// 매크로로 생성
let v = vec![1, 2, 3];

// 접근
let third: &i32 = &v[2];

// match로 안전하게 접근
match v.get(2) {
    Some(third) => println!("Third element: {}", third),
    None => println!("No third element"),
}

// 반복
for i in &v {
    println!("{}", i);
}

for i in &mut v {
    *i += 50;
}

// 길이와 비었는지 확인
println! ("{}", v.len());
println! ("{}", v.is_empty());

// 제거
v.pop();
v.remove(0);
```

### String
```rust
// 생성
let mut s = String::new();
let s = "initial contents". to_string();
let s = String::from("initial contents");

// 추가
let mut s = String::from("Hello");
s.push(' ');           // 단일 문자
s.push_str("World");   // 문자열

// 연결
let s1 = String::from("tic");
let s2 = String::from("tac");
let s3 = String::from("toe");
let s = s1 + "-" + &s2 + "-" + &s3;  // s1은 소유권을 잃음

let s = format!("{}-{}-{}", s1, s2, s3);  // 더 간단

// 반복
for c in "नमस्ते".chars() {
    println!("{}", c);
}

for b in "नमस्ते".bytes() {
    println! ("{}", b);
}

// 길이
println!("{}", s.len());  // 바이트 수
println!("{}", s.chars().count());  // 문자 수
```

### HashMap
```rust
use std::collections::HashMap;

// 생성
let mut scores = HashMap::new();
scores.insert(String::from("Blue"), 10);
scores.insert(String::from("Yellow"), 50);

// 초기값과 함께 생성
let teams = vec![String::from("Blue"), String::from("Yellow")];
let initial_scores = vec![10, 50];
let mut scores: HashMap<_, _> = teams.into_iter()
    .zip(initial_scores.into_iter())
    . collect();

// 접근
let team_name = String::from("Blue");
let score = scores.get(&team_name);

match score {
    Some(s) => println!("{}", s),
    None => println! ("Team not found"),
}

// 반복
for (key, value) in &scores {
    println!("{}: {}", key, value);
}

// 업데이트
scores.insert(String::from("Blue"), 25);  // 덮어쓰기

// entry API로 조건부 업데이트
scores.entry(String::from("Yellow")).or_insert(50);

// 값 기반 업데이트
let text = "hello world wonderful world";
let mut map = HashMap::new();

for word in text.split_whitespace() {
    let count = map.entry(word). or_insert(0);
    *count += 1;
}

println!("{:?}", map);
```

---

## 제네릭

### 함수 제네릭
```rust
fn largest<T: PartialOrd + Copy>(list: &[T]) -> T {
    let mut largest = list[0];

    for &item in list.iter() {
        if item > largest {
            largest = item;
        }
    }

    largest
}

fn main() {
    let numbers = vec![34, 50, 25, 100, 65];
    println!("The largest number is {}", largest(&numbers));

    let chars = vec!['y', 'm', 'a', 'q'];
    println!("The largest char is {}", largest(&chars));
}
```

### 구조체 제네릭
```rust
struct Point<T> {
    x: T,
    y: T,
}

struct Point2<T, U> {
    x: T,
    y: U,
}

fn main() {
    let integer = Point { x: 5, y: 10 };
    let float = Point { x: 1.0, y: 4.0 };
    let mixed = Point2 { x: 5, y: 4.0 };
}

impl<T> Point<T> {
    fn x(&self) -> &T {
        &self.x
    }
}

// 특정 타입에만 메서드 구현
impl Point<f32> {
    fn distance_from_origin(&self) -> f32 {
        (self.x.powi(2) + self.y.powi(2)).sqrt()
    }
}
```

### 열거형 제네릭
```rust
enum Option<T> {
    Some(T),
    None,
}

enum Result<T, E> {
    Ok(T),
    Err(E),
}
```

---

## 트레이트

### 트레이트 정의
```rust
pub trait Summarizable {
    fn summarize(&self) -> String;
}

pub struct NewsArticle {
    pub headline: String,
    pub location: String,
    pub author: String,
    pub content: String,
}

impl Summarizable for NewsArticle {
    fn summarize(&self) -> String {
        format!("{}, by {} ({})", self.headline, self. author, self.location)
    }
}

pub struct Tweet {
    pub username: String,
    pub content: String,
    pub reply: bool,
    pub retweet: bool,
}

impl Summarizable for Tweet {
    fn summarize(&self) -> String {
        format!("{}: {}", self.username, self. content)
    }
}
```

### 기본 구현
```rust
pub trait Summarizable {
    fn summarize(&self) -> String {
        String::from("(Read more...)")
    }
}

impl Summarizable for NewsArticle {}
```

### 트레이트 경계
```rust
// 함수에서 트레이트 경계
pub fn notify<T: Summarizable>(item: T) {
    println! ("Breaking news!  {}", item.summarize());
}

// 여러 트레이트
pub fn notify<T: Summarizable + Display>(item: T) {
    println!("Breaking news! {}", item.summarize());
}

```rust
// where 절 (더 읽기 쉬운 문법)
fn some_function<T, U>(t: T, u: U) -> i32
where
    T: Display + Clone,
    U: Clone + Debug,
{
    // 함수 본체
    0
}
```

### 트레이트 객체
```rust
pub trait Summarizable {
    fn summarize(&self) -> String;
}

// 트레이트 객체 사용
pub fn notify(item: &dyn Summarizable) {
    println!("Breaking news!  {}", item.summarize());
}

// Vec에 다양한 타입 저장
let mut posts: Vec<Box<dyn Summarizable>> = vec! [];
posts.push(Box::new(newsarticle));
posts.push(Box::new(tweet));

for post in posts. iter() {
    println!("{}", post.summarize());
}
```

### 고급 트레이트 기능

#### 연관 타입
```rust
pub trait Iterator {
    type Item;

    fn next(&mut self) -> Option<Self::Item>;
}

impl Iterator for Counter {
    type Item = u32;

    fn next(&mut self) -> Option<Self::Item> {
        // ... 
    }
}
```

#### 기본 제네릭 타입 파라미터
```rust
trait Add<RHS = Self> {
    type Output;

    fn add(self, rhs: RHS) -> Self::Output;
}

// 기본값을 사용
impl Add for Point {
    type Output = Point;

    fn add(self, other: Point) -> Point {
        Point {
            x: self.x + other.x,
            y: self.y + other.y,
        }
    }
}

// 기본값을 오버라이드
impl Add<Millimeters> for Meters {
    type Output = Meters;

    fn add(self, other: Millimeters) -> Meters {
        Meters(self.0 + other.0 as f64 / 1000.0)
    }
}
```

#### 완전히 지정된 문법
```rust
trait Animal {
    fn baby_name() -> String;
}

struct Dog;

impl Dog {
    fn baby_name() -> String {
        String::from("Spot")
    }
}

impl Animal for Dog {
    fn baby_name() -> String {
        String::from("puppy")
    }
}

fn main() {
    println! ("A baby dog is called a {}", Dog::baby_name());
    // Dog의 baby_name() 호출 -> "Spot"

    // Animal 트레이트의 baby_name() 호출
    println!("A baby dog is called a {}", <Dog as Animal>::baby_name());
    // -> "puppy"
}
```

---

## 모듈 및 크레이트

### 모듈 시스템

#### 모듈 정의
```rust
// src/lib.rs 또는 src/main.rs

mod front_of_house {
    mod hosting {
        fn add_to_waitlist() {}
        fn seat_at_table() {}
    }

    mod serving {
        fn take_order() {}
        fn serve_order() {}
        fn take_payment() {}
    }
}
```

#### 모듈 트리
```
crate
 └── front_of_house
      ├── hosting
      │   ├── add_to_waitlist
      │   └── seat_at_table
      └── serving
          ├── take_order
          ├── serve_order
          └── take_payment
```

### 가시성 제어

#### pub 키워드
```rust
mod front_of_house {
    pub mod hosting {
        pub fn add_to_waitlist() {}
    }
}

pub fn eat_at_restaurant() {
    // 절대 경로
    crate::front_of_house::hosting::add_to_waitlist();

    // 상대 경로
    front_of_house::hosting::add_to_waitlist();
}
```

#### 개인 vs 공개
```rust
mod front_of_house {
    pub mod hosting {
        pub fn add_to_waitlist() {}
        fn seat_at_table() {}  // 비공개
    }
}

// hosting은 공개이지만 seat_at_table은 비공개
// crate::front_of_house::hosting::seat_at_table();  // 오류!
```

### use 키워드

#### 경로 가져오기
```rust
mod front_of_house {
    pub mod hosting {
        pub fn add_to_waitlist() {}
    }
}

use crate::front_of_house::hosting;
// 또는
use crate::front_of_house::hosting::add_to_waitlist;

pub fn eat_at_restaurant() {
    hosting::add_to_waitlist();
    // 또는
    add_to_waitlist();
}
```

#### as로 별칭 지정
```rust
use std::fmt::Result;
use std::io::Result as IoResult;

fn function1() -> Result { /*.. .*/ }
fn function2() -> IoResult<()> { /*...*/ }
```

#### pub use로 re-export
```rust
pub use crate::front_of_house::hosting;

// 이제 사용자는 이렇게 사용 가능:
// restaurant::hosting::add_to_waitlist();
```

#### 중첩 경로 사용
```rust
use std::io::{self, Write};
use std::{cmp::Ordering, io};

// 또는
use std::io::{self, Write};
```

#### 글롭 연산자
```rust
use std::collections::*;
// std::collections의 모든 공개 항목 가져오기
```

### 파일로 모듈 분리

#### 프로젝트 구조
```
restaurant/
├── Cargo.toml
└── src/
    ├── main.rs
    ├── lib.rs
    └── front_of_house/
        ├── mod.rs
        └── hosting. rs
```

#### src/lib.rs
```rust
pub mod front_of_house;

pub use front_of_house::hosting;
```

#### src/front_of_house/mod.rs
```rust
pub mod hosting;
```

#### src/front_of_house/hosting.rs
```rust
pub fn add_to_waitlist() {}

pub fn seat_at_table() {}
```

#### src/main.rs
```rust
use restaurant::hosting;

fn main() {
    hosting::add_to_waitlist();
}
```

### 크레이트

#### 바이너리 크레이트 vs 라이브러리 크레이트

**바이너리 크레이트:**
```
src/main.rs  // 진입점
```

**라이브러리 크레이트:**
```
src/lib.rs   // 라이브러리 루트
```

**둘 다 있는 경우:**
```
src/
├── main.rs      // 바이너리
├── lib.rs       // 라이브러리
└── bin/         // 추가 바이너리
    ├── b1.rs
    └── b2.rs
```

---

## 생명주기 (Lifetime)

### 생명주기란?
생명주기는 참조가 유효한 스코프를 명시적으로 나타냅니다.

### 함수에서의 생명주기
```rust
// 오류: 어느 참조가 반환되는지 명확하지 않음
fn longest(x: &str, y: &str) -> &str {
    if x.len() > y.len() {
        x
    } else {
        y
    }
}

// 생명주기 명시
fn longest<'a>(x: &'a str, y: &'a str) -> &'a str {
    if x.len() > y.len() {
        x
    } else {
        y
    }
}

fn main() {
    let string1 = String::from("long string");
    {
        let string2 = String::from("xyz");
        let result = longest(string1.as_str(), string2.as_str());
        println!("The longest string is {}", result);
    }
}
```

### 구조체에서의 생명주기
```rust
struct ImportantExcerpt<'a> {
    part: &'a str,
}

fn main() {
    let novel = String::from("Call me Ishmael.  Some years ago.. .");
    let first_sentence = novel.split('.').next().expect("Could not find a '.'");
    let i = ImportantExcerpt {
        part: first_sentence,
    };
}
```

### 생명주기 생략 규칙
```rust
// 규칙 1: 각 참조 파라미터가 고유한 생명주기를 가짐
fn first_word<'a>(s: &'a str) -> &'a str { /*...*/ }

// 규칙 2: 하나의 입력 생명주기가 있으면 출력 생명주기에 할당됨
fn first_word(s: &str) -> &str { /*...*/ }

// 규칙 3: 메서드에서 &self가 있으면 출력이 self의 생명주기를 받음
impl<'a> ImportantExcerpt<'a> {
    fn return_part(&self, announcement: &str) -> &str {
        announcement
    }
}
```

### 정적 생명주기
```rust
let s: &'static str = "I have a static lifetime. ";

fn longest<'a>(x: &'a str, y: &'static str) -> &'a str {
    x
}
```

---

## 클로저 (Closure)

### 기본 클로저
```rust
let expensive_closure = |num| {
    println!("calculating slowly...");
    thread::sleep(Duration::from_secs(2));
    num
};

expensive_closure(5);
```

### 타입 주석과 반환 타입
```rust
let add_one_v2 = |x: u32| -> u32 { x + 1 };
let add_one_v3 = |x| { x + 1 };
let add_one_v4 = |x| x + 1;

println!("{}", add_one_v2(5));
```

### 클로저와 소유권
```rust
let x = vec![1, 2, 3];

// 소유권을 가져옴
let equal_to_x = move |z| z == x;

// println!("{:?}", x);  // 오류!  x는 클로저로 이동됨
```

### Fn 트레이트들
```rust
// Fn: 변하지 않는 빌려온 참조로 호출
let x = vec![1, 2, 3];
let print_vec = || println!("{:?}", x);
print_vec();  // OK

// FnMut: 가변 빌려온 참조로 호출
let mut y = vec![1, 2, 3];
let mut add_to_list = |v: i32| y.push(v);
add_to_list(4);  // OK

// FnOnce: 소유권을 가져와서 호출
let z = vec![1, 2, 3];
let consume_vec = move || drop(z);
consume_vec();  // OK
// consume_vec();  // 오류! 이미 호출됨
```

---

## 스마트 포인터

### Box<T>
```rust
// 힙에 값 저장
let b = Box::new(5);
println!("b = {}", b);

// 재귀 타입 정의
enum List {
    Cons(i32, Box<List>),
    Nil,
}

use List::{Cons, Nil};
let list = Cons(1, Box::new(Cons(2, Box::new(Cons(3, Box::new(Nil))))));
```

### Deref 트레이트
```rust
use std::ops::Deref;

struct MyBox<T>(T);

impl<T> Deref for MyBox<T> {
    type Target = T;

    fn deref(&self) -> &T {
        &self.0
    }
}

fn main() {
    let x = 5;
    let y = MyBox::new(x);

    assert_eq!(5, *y);
}
```

### Rc<T> (참조 카운팅)
```rust
use std::rc::Rc;

enum List {
    Cons(i32, Rc<List>),
    Nil,
}

use List::{Cons, Nil};

fn main() {
    let a = Rc::new(Cons(5, Rc::new(Cons(10, Rc::new(Nil)))));
    let b = Cons(3, Rc::clone(&a));
    let c = Cons(4, Rc::clone(&a));
}
```

### RefCell<T>와 내부 가변성
```rust
use std::cell::RefCell;

struct MockMessenger {
    sent_messages: RefCell<Vec<String>>,
}

impl MockMessenger {
    fn send(&self, message: &str) {
        self.sent_messages.borrow_mut(). push(String::from(message));
    }
}

fn main() {
    let mock = MockMessenger {
        sent_messages: RefCell::new(vec![]),
    };

    mock.send("Hello");

    assert_eq!(mock.sent_messages.borrow().len(), 1);
}
```

---

## 동시성

### 스레드 생성
```rust
use std::thread;
use std::time::Duration;

fn main() {
    let handle = thread::spawn(|| {
        for i in 1..10 {
            println!("hi number {} from spawned thread!", i);
            thread::sleep(Duration::from_millis(1));
        }
    });

    for i in 1..5 {
        println!("hi number {} from main thread!", i);
        thread::sleep(Duration::from_millis(1));
    }

    handle.join(). unwrap();
}
```

### 스레드와 소유권
```rust
use std::thread;

fn main() {
    let v = vec![1, 2, 3];

    let handle = thread::spawn(move || {
        println!("Here's a vector: {:?}", v);
    });

    handle. join().unwrap();
}
```

### 메시지 전송 (MPSC)
```rust
use std::sync::mpsc;
use std::thread;

fn main() {
    let (tx, rx) = mpsc::channel();

    thread::spawn(move || {
        let val = String::from("hi");
        tx.send(val).unwrap();
    });

    let received = rx.recv().unwrap();
    println!("Got: {}", received);
}
```

### 여러 생산자
```rust
use std::sync::mpsc;
use std::thread;

fn main() {
    let (tx, rx) = mpsc::channel();

    let tx1 = tx.clone();
    thread::spawn(move || {
        let vals = vec![
            String::from("hi"),
            String::from("from"),
            String::from("the"),
            String::from("thread"),
        ];

        for val in vals {
            tx1.send(val).unwrap();
            thread::sleep(Duration::from_secs(1));
        }
    });

    for received in rx {
        println!("Got: {}", received);
    }
}
```

### Mutex<T> (상호 배제)
```rust
use std::sync::Mutex;

fn main() {
    let m = Mutex::new(5);

    {
        let mut num = m.lock().unwrap();
        *num = 6;
    }

    println! ("m = {:?}", m);
}
```

### Arc<T> (원자적 참조 카운팅)
```rust
use std::sync::{Arc, Mutex};
use std::thread;

fn main() {
    let counter = Arc::new(Mutex::new(0));
    let mut handles = vec![];

    for _ in 0..10 {
        let counter = Arc::clone(&counter);
        let handle = thread::spawn(move || {
            let mut num = counter.lock().unwrap();
            *num += 1;
        });
        handles.push(handle);
    }

    for handle in handles {
        handle. join().unwrap();
    }

    println!("Result: {}", *counter.lock(). unwrap());
}
```

---

## 패턴과 매칭 고급

### 패턴의 다양한 사용
```rust
// if let
let x = Some(5);
if let Some(5) = x {
    println!("five");
}

// while let
let mut stack = vec![1, 2, 3];
while let Some(top) = stack.pop() {
    println!("{}", top);
}

// for 루프
let v = vec! ['a', 'b', 'c'];
for (index, value) in v.iter().enumerate() {
    println!("{} is at index {}", value, index);
}

// let 문
let (x, y, z) = (1, 2, 3);

// 함수 파라미터
fn print_coordinates(&(x, y): &(i32, i32)) {
    println!("Current location: ({}, {})", x, y);
}
```

### 반박할 수 없는 패턴
```rust
// OK: 반박할 수 없는 패턴
let x = 5;

// 오류: 반박 가능한 패턴
// let Some(x) = some_option_value;

// OK: if let으로 반박 가능한 패턴 처리
if let Some(x) = some_option_value {
    println!("{}", x);
}
```

### 패턴 매칭 고급
```rust
match x {
    1 | 2 => println!("one or two"),
    3.. =5 => println!("three through five"),
    Some(50) => println!("fifty"),
    Some(n) if n % 2 == 0 => println!("matched even"),
    _ => println!("something else"),
}
```

---

## 테스트

### 기본 테스트
```rust
#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn it_works() {
        assert_eq!(2 + 2, 4);
    }

    #[test]
    fn it_fails() {
        panic!("Make this test fail");
    }
}
```

### 테스트 실행
```bash
cargo test               # 모든 테스트 실행
cargo test test_name     # 특정 테스트만 실행
cargo test -- --test-threads=1  # 단일 스레드로 실행
cargo test -- --nocapture       # 출력 표시
```

### 다양한 어설션
```rust
#[test]
fn it_works() {
    assert!(true);
    assert_eq!(2 + 2, 4);
    assert_ne!(2 + 2, 5);
}

#[test]
#[should_panic]
fn it_panics() {
    panic!("Oh no!");
}

#[test]
#[should_panic(expected = "Guess value must be")]
fn guess_too_small() {
    panic!("Guess value must be less than 100");
}
```

---

## 실용적인 예제

### 간단한 계산기
```rust
fn main() {
    let nums = vec![1, 2, 3, 4, 5];
    
    let sum: i32 = nums.iter(). sum();
    let product: i32 = nums.iter().product();
    let avg = sum as f64 / nums. len() as f64;
    
    println!("Sum: {}", sum);
    println!("Product: {}", product);
    println!("Average: {}", avg);
}
```

### 커맨드라인 인자 처리
```rust
use std::env;

fn main() {
    let args: Vec<String> = env::args().collect();
    
    if args.len() < 2 {
        eprintln! ("Usage: {} <query> <filename>", args[0]);
        return;
    }
    
    let query = &args[1];
    let filename = &args[2];
    
    println!("Searching for '{}'", query);
    println!("In file '{}'", filename);
}
```

### 파일 읽기
```rust
use std::fs;

fn main() {
    let contents = fs::read_to_string("poem.txt")
        .expect("Should have been able to read the file");
    
    println!("With text:\n{}", contents);
}
```

### 간단한 웹 서버
```rust
use std::fs;
use std::io::{prelude::*, BufReader};
use std::net::{TcpListener, TcpStream};

fn main() {
    let listener = TcpListener::bind("127.0. 0.1:7878")
        .expect("Failed to bind");
    
    for stream in listener.incoming() {
        let stream = stream.expect("Connection failed");
        handle_connection(stream);
    }
}

fn handle_connection(mut stream: TcpStream) {
    let buf_reader = BufReader::new(&mut stream);
    let request_line = buf_reader.lines(). next().unwrap().unwrap();
    
    let response = "HTTP/1.1 200 OK\r\n\r\nHello World! ";
    stream.write_all(response.as_bytes()).unwrap();
}
```

---

## 유용한 팁과 트릭

### 타입 추론 강제하기
```rust
let v = Vec::new();  // 오류!  타입을 알 수 없음
let v: Vec<i32> = Vec::new();  // OK
let v = vec![1, 2, 3];  // 매크로로 OK
```

### 디버깅 프린트
```rust
#[derive(Debug)]
struct Rectangle {
    width: u32,
    height: u32,
}

let rect = Rectangle { width: 30, height: 50 };
println!("{:? }", rect);  // 한 줄 출력
println!("{:#?}", rect);  // 예쁜 출력
```

### 파이프라인 연산
```rust
let number = Some(4);
let number = number
    .map(|x| x + 1)
    .map(|x| x * 2)
    .and_then(|x| Some(x));
```

### 함수형 스타일
```rust
let numbers = vec![1, 2, 3, 4, 5];

let result: Vec<i32> = numbers
    .iter()
    .filter(|x| x % 2 == 0)
    .map(|x| x * x)
    .collect();

println!("{:?}", result);  // [4, 16]
```

---

## 참고 자료

- [공식 Rust 문서](https://www.rust-lang.org)
- [The Rust Programming Language (한국어)](https://rinthel.github.io/rust-lang-book-ko/)
- [Rust by Example](https://doc.rust-lang.org/rust-by-example/)
- [Rust std lib 문서](https://doc.rust-lang.org/std/)
