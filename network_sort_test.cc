extern "C" {
#include "network_sort.h"
}

#include <cstdlib>
#include <string>

#include "gtest/gtest.h"

static std::string ATS(int a[7]) {
  char buf[20];
  sprintf(buf, "%d,%d,%d,%d,%d,%d,%d", a[0], a[1], a[2], a[3], a[4], a[5], a[6]);
  return buf;
}
static int S1[7] = {0,1,2,3,4,5,6};
static int S2[7] = {6,5,4,3,2,1,0};
static int S3[7] = {5,3,1,6,0,4,2};
class SortTest : public ::testing::Test {
  void SetUp() {
    for (int i = 0; i < 7; ++i) {
      s1[i] = S1[i];
      s2[i] = S2[i];
      s3[i] = S3[i];
    }
  }
 protected:
  int s1[7];
  int s2[7];
  int s3[7];
};

TEST_F(SortTest, SelfTest) {
  EXPECT_EQ("0,1,2,3,4,5,6", ATS(s1));
  EXPECT_EQ(ATS(S1), ATS(s1));
}

TEST_F(SortTest, Sort01) {
  network_sort(s1, 0);
  EXPECT_EQ(ATS(S1), ATS(s1));
  network_sort(s1, 1);
  EXPECT_EQ(ATS(S1), ATS(s1));
}

TEST_F(SortTest, Sort2) {
  network_sort(s1, 2);
  EXPECT_EQ(ATS(S1), ATS(s1));
  network_sort(s2, 2);
  EXPECT_EQ("5,6,4,3,2,1,0", ATS(s2));
  network_sort(s3, 2);
  EXPECT_EQ("3,5,1,6,0,4,2", ATS(s3));
}

TEST_F(SortTest, Sort3) {
  network_sort(s1, 3);
  EXPECT_EQ(ATS(S1), ATS(s1));
  network_sort(s2, 3);
  EXPECT_EQ("4,5,6,3,2,1,0", ATS(s2));
  network_sort(s3, 3);
  EXPECT_EQ("1,3,5,6,0,4,2", ATS(s3));
}

TEST_F(SortTest, Sort4) {
  network_sort(s1, 4);
  EXPECT_EQ(ATS(S1), ATS(s1));
  network_sort(s2, 4);
  EXPECT_EQ("3,4,5,6,2,1,0", ATS(s2));
  network_sort(s3, 4);
  EXPECT_EQ("1,3,5,6,0,4,2", ATS(s3));
}

TEST_F(SortTest, Sort5) {
  network_sort(s1, 5);
  EXPECT_EQ(ATS(S1), ATS(s1));
  network_sort(s2, 5);
  EXPECT_EQ("2,3,4,5,6,1,0", ATS(s2));
  network_sort(s3, 5);
  EXPECT_EQ("0,1,3,5,6,4,2", ATS(s3));
}

TEST_F(SortTest, Sort6) {
  network_sort(s1, 6);
  EXPECT_EQ(ATS(S1), ATS(s1));
  network_sort(s2, 6);
  EXPECT_EQ("1,2,3,4,5,6,0", ATS(s2));
  network_sort(s3, 6);
  EXPECT_EQ("0,1,3,4,5,6,2", ATS(s3));
}

TEST(SortRandomTest, Sort2) {
  static const int N = 2;
  srand(N);
  for (int i = 0; i < 1000; ++i) {
    int a[N];
    for (int j = 0; j < N; ++j) {
      a[j] = rand() % 100;
    }
    network_sort(a, N);
    for (int j = 1; j < N; ++j) {
      EXPECT_LE(a[j-1], a[j]);
    }
  }
}

TEST(SortRandomTest, Sort3) {
  static const int N = 3;
  srand(N);
  for (int i = 0; i < 1000; ++i) {
    int a[N];
    for (int j = 0; j < N; ++j) {
      a[j] = rand() % 100;
    }
    network_sort(a, N);
    for (int j = 1; j < N; ++j) {
      EXPECT_LE(a[j-1], a[j]);
    }
  }
}

TEST(SortRandomTest, Sort4) {
  static const int N = 4;
  srand(N);
  for (int i = 0; i < 1000; ++i) {
    int a[N];
    for (int j = 0; j < N; ++j) {
      a[j] = rand() % 100;
    }
    network_sort(a, N);
    for (int j = 1; j < N; ++j) {
      EXPECT_LE(a[j-1], a[j]);
    }
  }
}

TEST(SortRandomTest, Sort5) {
  static const int N = 5;
  srand(N);
  for (int i = 0; i < 1000; ++i) {
    int a[N];
    for (int j = 0; j < N; ++j) {
      a[j] = rand() % 100;
    }
    network_sort(a, N);
    for (int j = 1; j < N; ++j) {
      EXPECT_LE(a[j-1], a[j]);
    }
  }
}

TEST(SortRandomTest, Sort6) {
  static const int N = 6;
  srand(N);
  for (int i = 0; i < 1000; ++i) {
    int a[N];
    for (int j = 0; j < N; ++j) {
      a[j] = rand() % 100;
    }
    network_sort(a, N);
    for (int j = 1; j < N; ++j) {
      EXPECT_LE(a[j-1], a[j]);
    }
  }
}
