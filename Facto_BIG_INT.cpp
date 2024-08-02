#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cin;
using std::cout;
using std::vector;
using std::string;
using std::max;

class Factorial {
 public:

  Factorial(const string& snum);
  Factorial(const int& l);
  Factorial(vector<int> n);

  // Только так, ибо для факторила этого хватит
  friend bool operator>(Factorial x, Factorial y);
  friend Factorial operator+(Factorial a, Factorial b);
  friend Factorial operator-(Factorial a, Factorial b);
  friend Factorial operator*(Factorial a, Factorial b);
  friend Factorial operator/(Factorial a, int b);
  friend bool operator==(Factorial a, Factorial b);
  Factorial& operator=(const Factorial& n);

  // Выравнивание длины для умножения
  void Alignment(vector<int>& v, size_t len);
  // Перенос за ячейку массива
  vector<int> Finalize(vector<long long>& res);
  void Finalize();
  // Чистка нулей
  void ClearZero();
  void ClearZero(vector<int>& a);
  int Get_size();
  // Чистка Минусов, иногда при операциях у Каратсубы есть отрицательные, так как я работаю с большими числами, то нужно 
  // пробегать и брать ед-ку у след числа
  void CheckMinus();
  void CheckMinus(vector<int>& c);
  void CheckMinus(Factorial& n);
  void PrintF();
  vector<int> GetThis();
  // Главное, Ы
  void FactTree();
  Factorial ProdTree(Factorial l, Factorial r);

 private:
  vector<int> num;
  int lenN = 0;
  // Костыли, для проверки
  const vector<int> AA = {1};
  const vector<int> BB = {0};
  const vector<int> CC = {2};
  int pos = 0;
  // База, макс число которое находится в одной ячейки массива, можно увеличить(Время будет быстрее макс размер 10^9), ну нужно следить за переполнением, Ы
  int base = 1000;
  vector<int> Naive_mul(vector<int> a, vector<int> b);
  vector<int> Karatsuba_mul(vector<int> x, vector<int> y);
};

vector<int> Factorial::GetThis() {
  return num;
}

Factorial& Factorial::operator=(const Factorial& n) {
  if (this != &n) // защита от неправильного самоприсваивания
  {
    num = n.num;
  }
  return *this;
}

int Factorial::Get_size() {
  return num.size();
}

Factorial::Factorial(vector<int> n) {
  num = n;
}

Factorial::Factorial(const int& l) {
  num.resize(l);
}

void Factorial::Alignment(vector<int>& v, size_t len) {
  // прикольная проверка степени двойки
  while (len & (len - 1)) {
    ++len;
  }

  v.resize(len);
}

Factorial operator+(Factorial a, Factorial b) {
  Factorial c(max(a.num.size(), b.num.size()) + 1);
  for (int i = 0; i < max(a.num.size(), b.num.size()); ++i) { //сначала сложим числа поразрядно,
    if (a.num.size() <= i && b.num.size() > i) {
      c.num[i] = b.num[i];
    } else if (b.num.size() <= i && a.num.size() > i) {
      c.num[i] = a.num[i];
    } else {
      c.num[i] = a.num[i] + b.num[i];
    }
  }
  for (int i = 0; i < c.num.size() - 1; ++i) { //а затем поочередно выполним переносы
    if (c.num[i] >= 1000) { //для каждого разряда
      c.num[i] -= 1000;
      c.num[i + 1]++;
    }
  }
  return c;
}

bool operator>(Factorial a, Factorial b) {
  if (a.num.size() != b.num.size()) {
    if (a.num.size() > b.num.size()) {
      return true;
    } else {
      return false;
    }
  } else {
    int i = a.num.size() - 1;
    while (i > 0 && a.num[i] == b.num[i]) {
      --i;
    }
    if (a.num[i] == b.num[i]) {
      return false;
    }
    if (a.num[i] > b.num[i]) {
      return true;
    } else {
      return false;
    }
  }
}

Factorial::Factorial(const string& snum) {
  unsigned int dig = 1;
  unsigned int dig_size = 10;
  int n = 0;
  for (auto it = snum.crbegin(); it != snum.crend(); ++it) {
    n += (*it - '0') * dig;
    dig *= dig_size;
    if (dig == base) {
      num.push_back(n);
      n = 0;
      dig = 1;
    }
  }
  if (n != 0) {
    num.push_back(n);
  }
}

void Factorial::ClearZero() {
  int len = num.size();
  int i = 1;
  while ((!num[len - i]) && (len - i > 0)) {
    ++i;
    num.pop_back();
  }
}

void Factorial::ClearZero(vector<int>& a) {
  int len = a.size();
  int i = 1;
  while ((!a[len - i]) && (len - i > 0)) {
    ++i;
    a.pop_back();
  }
}

vector<int> Factorial::Finalize(vector<long long>& res) {
  vector<int> resS(res.size());
  for (auto i = 0; i < res.size() - 1; ++i) {
    res[i + 1] += (res[i] / 1000);
    res[i] %= 1000;
  }
  for (auto i = 0; i < res.size(); ++i) {
    resS[i] = (int) res[i];
  }
  return resS;
}

void Factorial::Finalize() {
  for (auto i = 0; i < num.size(); ++i) {
    num[i + 1] += num[i] / base;
    num[i] %= base;
  }
}

Factorial operator-(Factorial a, Factorial b) {
  Factorial c(max(a.num.size(), b.num.size()));
  for (int i = 0; i < b.num.size(); ++i) {
    c.num[i] = a.num[i] - b.num[i];
  }
  for (int i = 0; i < a.num.size() - 1; ++i) {
    if (c.num[i] < 0) {
      c.num[i] += 1000;
      c.num[i + 1]--;
    }
  }
  return c;
}

void Factorial::CheckMinus() {
  for (int i = 0; i < num.size() - 1; ++i) {
    if (num[i] < 0 && num[i] > -1000) {
      num[i] += 1000;
      num[i + 1]--;
    } else if (num[i] < -1000 && num[i] > -10000) {
      num[i] += 10000;
      num[i + 1] = num[i + 1] - 10;
    } else if (num[i] < -10000 && num[i] > -100000) {
      num[i] += 100000;
      num[i + 1] = num[i + 1] - 100;
    }
  }
}

void Factorial::CheckMinus(Factorial& n) {
  for (int i = 0; i < n.num.size() - 1; ++i) {
    if (n.num[i] < 0 && n.num[i] > -1000) {
      n.num[i] += 1000;
      n.num[i + 1]--;
    } else if (n.num[i] < -1000 && n.num[i] > -10000) {
      n.num[i] += 10000;
      n.num[i + 1] = n.num[i + 1] - 10;
    } else if (n.num[i] < -10000 && n.num[i] > -100000) {
      n.num[i] += 100000;
      n.num[i + 1] = n.num[i + 1] - 100;
    }
  }
}

void Factorial::CheckMinus(vector<int>& c) {
  for (int i = 0; i < c.size() - 1; ++i) {
    if (c[i] < 0 && c[i] > -1000) {
      c[i] += 1000;
      c[i + 1]--;
    } else if (c[i] < -1000 && c[i] > -10000) {
      c[i] += 10000;
      c[i + 1] = c[i + 1] - 10;
    } else if (c[i] < -10000 && c[i] > -100000) {
      c[i] += 100000;
      c[i + 1] = c[i + 1] - 100;
    }
  }
}

vector<int> Factorial::Naive_mul(vector<int> a, vector<int> b) {
  CheckMinus(a);
  CheckMinus(b);
  auto len = a.size();
  long long carry = 0;
  vector<long long> res(2 * len);
  for (int i = 0; i < b.size(); ++i) {
    for (int j = 0; j < a.size(); ++j) {
      res[i + j] += (long long) a[i] * (long long) b[j];
    }
  }
  return Finalize(res);
}

vector<int> Factorial::Karatsuba_mul(vector<int> x, vector<int> y) {
  auto n = max(x.size(), y.size());
  Alignment(x, n);
  Alignment(y, n);
  auto len = x.size();
  vector<int> res(2 * len);
  if (len <= 128) {
    return Naive_mul(x, y);
  }
  auto k = len / 2;
  vector<int> Xr{x.begin(), x.begin() + k};
  vector<int> Xl{x.begin() + k, x.end()};
  vector<int> Yr{y.begin(), y.begin() + k};
  vector<int> Yl{y.begin() + k, y.end()};
  vector<int> P1 = Karatsuba_mul(Xl, Yl);
  vector<int> P2 = Karatsuba_mul(Xr, Yr);
  vector<int> Xlr(k);
  vector<int> Ylr(k);
  for (int i = 0; i < k; ++i) {
    Xlr[i] = Xl[i] + Xr[i];
    Ylr[i] = Yl[i] + Yr[i];
  }
  vector<int> P3 = Karatsuba_mul(Xlr, Ylr);
  for (auto i = 0; i < len; ++i) {
    P3[i] -= P2[i] + P1[i];
  }
  for (auto i = 0; i < len; ++i) {
    res[i] = P2[i];
  }
  for (auto i = len; i < 2 * len; ++i) {
    res[i] = P1[i - len];
  }
  for (auto i = k; i < len + k; ++i) {
    res[i] += P3[i - k];
  }
  CheckMinus(res);
  return res;
}

Factorial operator*(Factorial a, Factorial b) {
  auto n = max(a.num.size(), b.num.size());
  a.Alignment(a.num, n);
  b.Alignment(b.num, n);
  Factorial res(res.Karatsuba_mul(a.num, b.num));
  res.CheckMinus(res);
  res.Finalize();
  return res;
}

void Factorial::PrintF() {
  int len = num.size();
  int i = 1;
  while (!num[len - i]) {
    ++i;
  }
  cout << num[len - i];
  ++i;
  while (len - i >= 0) {
    if (num[len - i] == 0) {
      cout << "000";
    } else {
      if (num[len - i] < 10) {
        cout << "00";
      } else if (num[len - i] < 100 && num[len - i] >= 10) {
        cout << "0";
      }

      cout << num[len - i];
    }

    ++i;
  }
  cout << '\n';
}

Factorial operator/(Factorial a, int b) {
  for (int i = a.num.size(); i >= 0; i--) {
    if (i) {
      a.num[i - 1] += (a.num[i] % b) * 1000;
    }
    a.num[i] /= b;
  }
  return a;
}

bool operator==(Factorial a, Factorial b) {
  if (a.num.size() != b.num.size()) {
    return false;
  } else {
    int i = a.num.size() - 1;
    while (i >= 0 && a.num[i] == b.num[i]) {
      --i;
    }
    if (i <= 0 && a.num[0] == b.num[0]) {
      return true;
    } else {
      return false;
    }
  }
}

void Factorial::FactTree() {
  if (num == BB) {
    return;
  }
  if (num == AA || num == CC)
    return;
  *this = ProdTree(CC, num);
  CheckMinus();
}

Factorial Factorial::ProdTree(Factorial l, Factorial r) {
  l.ClearZero();
  r.ClearZero();
  if (l > r) {
    return AA;
  }
  if (l == r) {
    return l;
  }
  Factorial intre = r - l;
  intre.ClearZero();
  if (intre == AA) {
    Factorial multT = l * r;
    return multT;
  }
  Factorial s = l + r;
  Factorial m = s / 2;
  s.ClearZero();
  m.ClearZero();
  Factorial mult = ProdTree(l, m) * ProdTree(m + AA, r);
  return (mult);
}

int main() {
  string Num;
  cin >> Num;
  Factorial kek(Num);
  kek.PrintF();
  kek.FactTree();
  cout << kek.Get_size() << "\n";
  kek.PrintF();
  return 0;
}