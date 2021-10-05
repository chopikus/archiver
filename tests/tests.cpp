#include "../archiver/archiver.h"
#include "../min_priority_queue/min_priority_queue.h"
#include "../trie/trie.h"
#include "../writer/writer.h"
#include <gtest/gtest.h>
#include <string>
#include <utility>
#include <vector>

TEST(MinPriorityQueue, PushAndPop1) {
    MinPriorityQueue q;
    q.Push(500, 5);
    q.Push(600, 5);
    q.Push(300, 3);
    q.Push(100, 1);     
    q.Push(200, 1);
    std::vector<KeyPriority> v;
    std::vector<KeyPriority> expected = {{100, 1}, {200, 1}, {300, 3}, {500, 5}, {600, 5}};
    while (!q.Empty()) {
        v.push_back(q.Top());
        KeyPriority kp1 = q.Top();
        KeyPriority kp2 = q.Pop();
        ASSERT_EQ(kp1, kp2);
    }
    ASSERT_EQ(v, expected);
}

TEST(MinPriorityQueue, PushAndPop2) {
    MinPriorityQueue q;
    q.Push(5, 5);
    q.Push(6, 4);
    q.Push(3, 3);
    q.Push(1, 2);     
    q.Push(2, 1);
    q.Push(228, 0);
    std::vector<KeyPriority> v;
    std::vector<KeyPriority> expected = {{228, 0}, {2, 1}, {1, 2}, {3, 3}, {6, 4}, {5, 5}};
    while (!q.Empty()) {
        v.push_back(q.Top());
        KeyPriority kp1 = q.Top();
        KeyPriority kp2 = q.Pop();
        ASSERT_EQ(kp1, kp2);
    }
    ASSERT_EQ(v, expected);
}

TEST(MinPriorityQueue, PushAndPopMAX) {
    MinPriorityQueue q;
    std::vector<KeyPriority> expected;
    for (uint16_t i = 0; i < 50000; ++i) {
        expected.push_back({i, i});
    }
    std::reverse(expected.begin(), expected.end());
    for (uint16_t i = 0; i < 50000; ++i) {
        q.Push(expected[i].key, expected[i].priority);
    }
    std::reverse(expected.begin(), expected.end());
    std::vector<KeyPriority> v;
    while (!q.Empty()) {
        v.push_back(q.Top());
        KeyPriority kp1 = q.Top();
        KeyPriority kp2 = q.Pop();
        ASSERT_EQ(kp1, kp2);
    }
    ASSERT_EQ(v, expected);
}

TEST(Trie, AddAndCheck1) {
    Trie trie(9);
    trie.AddChild(0, 1);
    trie.AddChild(0, 2);
    trie.AddChild(1, 3);
    trie.AddChild(1, 4);
    trie.AddChild(2, 7);
    trie.AddChild(7, 8);
    std::vector<std::pair<uint16_t, std::string> > expected = {{3, "00"}, {4, "01"}, {8, "100"}};
    ASSERT_EQ(expected, trie.LeavesFrom(0));
}

TEST(Trie, AddAndCheckMAX) {
    Trie trie(500);
    for (size_t i = 0; i < 1000; ++i) {
        trie.AddChild(i, i + 1);
    }
    std::vector<std::pair<uint16_t, std::string> > expected;
    std::string s;
    for (uint16_t j = 0; j < 499; ++j) {
        s += "0";
    }
    expected = {{499, s}};
    ASSERT_EQ(expected, trie.LeavesFrom(0));
}

constexpr size_t MAX_EVER_POSSIBLE_SIZE = 1024;

std::vector<char> read_bytes(std::string file_path) {
    std::vector<char> result;
    char buffer[MAX_EVER_POSSIBLE_SIZE] = {0};
    std::ifstream infile(file_path);
    infile.seekg(0, std::ios::end);
    intmax_t length = infile.tellg();
    infile.seekg(0, std::ios::beg);
    if (length == -1)
        return result;
    infile.read(buffer, length);
    for (size_t i = 0; i < static_cast<size_t>(length); ++i) {
        result.push_back(buffer[i]);
    }
    return result;
}

TEST(Writer, WriteOneByte) {
    Writer writer("writer_test1.bin");
    for (size_t i = 0; i < 8; ++i) {
        writer.Write1(i < 4);   
    }
    writer.Finish();
    std::vector<char> p1 = read_bytes("writer_test1.bin");
    std::vector<char> p2 = read_bytes("../tests/writer_ans1.bin");
    size_t len1 = p1.size();
    size_t len2 = p2.size();
    ASSERT_EQ(len1, 1);
    ASSERT_EQ(len2, 1);
    unsigned char b1 = p1[0];
    unsigned char b2 = p2[0];
    ASSERT_EQ(b1, 240);
    ASSERT_EQ(b2, 240);
    ASSERT_EQ(b1, b2);
}

TEST(Writer, Write9BitsTwice) {
    Writer writer("writer_test2.bin");
    writer.Write9(257);
    writer.Write9(385);
    writer.Finish();
    std::vector<char> p1 = read_bytes("writer_test2.bin");    
    std::vector<char> p2 = read_bytes("../tests/writer_ans2.bin"); 
    ASSERT_EQ(p1.size(), 3);
    ASSERT_EQ(p2.size(), 3);
    for (size_t i = 0; i < 2; ++i) {
        ASSERT_EQ(p1[i], p2[i]);
    }
}

TEST(Archiver, Compress) {
    Archiver archiver("../tests/a");
    Writer writer("a_compressed");
    archiver.CompressTo(writer, true);
    writer.Finish();
    Writer wr("compress_test");
    wr.Write9(4);
    wr.Write9(97);
    wr.Write9(258);
    wr.Write9(256);
    wr.Write9(257);
    wr.Write9(1);
    wr.Write9(1);
    wr.Write9(2);
    wr.WriteAny("0");
    wr.WriteAny("110");
    wr.WriteAny("0");
    wr.WriteAny("0");
    wr.WriteAny("0");
    wr.WriteAny("0");
    wr.WriteAny("0");
    wr.WriteAny("10");
    wr.Finish();
    std::vector<char> p1 = read_bytes("a_compressed");
    std::vector<char> p2 = read_bytes("compress_test");
    ASSERT_EQ(p1.size(), p2.size());
    for (size_t i = 0; i < p1.size(); ++i) {
        unsigned char u1 = p1[i];
        unsigned char u2 = p2[i];
        ASSERT_EQ(u1, u2);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

