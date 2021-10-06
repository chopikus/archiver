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
    std::vector<std::pair<Key, Priority>> v;
    std::vector<std::pair<Key, Priority>> expected = {{100, 1}, {200, 1}, {300, 3}, {500, 5}, {600, 5}};
    while (!q.Empty()) {
        v.push_back(q.Top());
        std::pair<Key, Priority> kp1 = q.Top();
        std::pair<Key, Priority> kp2 = q.Pop();
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
    std::vector<std::pair<Key, Priority>> v;
    std::vector<std::pair<Key, Priority>> expected = {{228, 0}, {2, 1}, {1, 2}, {3, 3}, {6, 4}, {5, 5}};
    while (!q.Empty()) {
        v.push_back(q.Top());
        std::pair<Key, Priority> kp1 = q.Top();
        std::pair<Key, Priority> kp2 = q.Pop();
        ASSERT_EQ(kp1, kp2);
    }
    ASSERT_EQ(v, expected);
}

TEST(MinPriorityQueue, PushAndPopMAX) {
    MinPriorityQueue q;
    std::vector<std::pair<Key, Priority> > expected;
    for (uint16_t i = 0; i < 50000; ++i) {
        expected.push_back({i, i});
    }
    std::reverse(expected.begin(), expected.end());
    for (uint16_t i = 0; i < 50000; ++i) {
        q.Push(expected[i].first, expected[i].second);
    }
    std::reverse(expected.begin(), expected.end());
    std::vector<std::pair<Key, Priority> > v;
    while (!q.Empty()) {
        v.push_back(q.Top());
        std::pair<Key, Priority> kp1 = q.Top();
        std::pair<Key, Priority> kp2 = q.Pop();
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
    std::vector<std::pair<uint16_t, uint16_t> > expected = {{3, 2}, {4, 2}, {8, 3}};
    ASSERT_EQ(expected, trie.LeavesFrom(0));
}

TEST(Trie, AddAndCheckMAX) {
    Trie trie(500);
    for (size_t i = 0; i < 1000; ++i) {
        trie.AddChild(i, i + 1);
    }
    std::vector<std::pair<uint16_t, uint16_t> > expected;
    expected = {{499, 499}};
    ASSERT_EQ(expected, trie.LeavesFrom(0));
}

TEST(Reader, ReadBinaryFile) {
    const std::vector<unsigned char> expected_data = {
        0xAA, 0xAA, 0xAA, 0xAA,
        0xBB, 0xBB, 0xBB, 0xBB,
        0xCC, 0xCC, 0xCC, 0xCC
    };

    Reader reader("../tests/mock/reader_test1.bin");
    for (size_t i = 0; i < expected_data.size(); ++i) {
        ASSERT_FALSE(reader.IsEOF());
        auto read_byte = reader.Read8();
        ASSERT_EQ(read_byte, expected_data[i]);
    }

    ASSERT_TRUE(reader.IsEOF());
}

constexpr size_t MAX_EVER_POSSIBLE_SIZE = 1024;

std::vector<char> ReadBytes(std::string file_path) {
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
    std::vector<char> p1 = ReadBytes("writer_test1.bin");
    std::vector<char> p2 = ReadBytes("../tests/mock/writer_ans1.bin");
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
    std::vector<char> p1 = ReadBytes("writer_test2.bin");    
    std::vector<char> p2 = ReadBytes("../tests/mock/writer_ans2.bin"); 
    ASSERT_EQ(p1.size(), 3);
    ASSERT_EQ(p2.size(), 3);
    for (size_t i = 0; i < 2; ++i) {
        ASSERT_EQ(p1[i], p2[i]);
    }
}

TEST(Archiver, Compress) {
    Archiver archiver({"../tests/mock/a"});
    archiver.CompressTo("a_compressed");
    Writer wr("../tests/mock/compress_test");
    wr.Write9(4);
    wr.Write9(97);
    wr.Write9(258);
    wr.Write9(256);
    wr.Write9(257);
    wr.Write9(1);
    wr.Write9(1);
    wr.Write9(2);
    wr.WriteAny(0, 1);
    wr.WriteAny(0b110, 3);
    wr.WriteAny(0, 1);
    wr.WriteAny(0, 1);
    wr.WriteAny(0, 1);
    wr.WriteAny(0, 1);
    wr.WriteAny(0, 1);
    wr.WriteAny(0b10, 2);
    wr.Finish();
    std::vector<char> p1 = ReadBytes("a_compressed");
    std::vector<char> p2 = ReadBytes("../tests/mock/compress_test");
    ASSERT_EQ(p1.size(), p2.size());
    for (size_t i = 0; i < p1.size(); ++i) {
        unsigned char u1 = p1[i];
        unsigned char u2 = p2[i];
        ASSERT_EQ(u1, u2);
    }
}

TEST(Archiver, CompressAndDecompress) {
    std::vector<std::string> names = {"a", "ab", "abc"};
    std::vector<std::string> archive_paths = {"../tests/mock/a", "../tests/mock/ab", "../tests/mock/abc"};
    Archiver archiver(archive_paths);
    archiver.CompressTo("compressed.bin");
    Archiver d({"compressed.bin"});
    d.Decompress();
    for (size_t i = 0; i < names.size(); ++i) {
        std::vector<char> v1 = ReadBytes(archive_paths[i]);
        std::vector<char> v2 = ReadBytes(names[i]);
        ASSERT_EQ(v1.size(), v2.size());
        for (size_t j = 0; j < v1.size(); ++j) {
            ASSERT_EQ(v1[j], v2[j]);
        }
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

