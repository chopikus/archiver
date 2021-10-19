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

TEST(Reader, ReadByBit) {
    const std::vector<bool> expected_data = {
        1,0,0,0,0,0,0,0,
        1,1,0,0,0,0,0,0,
        1,1,1,0,0,0,0,0,
        1,0,0,0,0,0,0,0,
        0,1,0,0,0,0,0,0 
    };
    Reader reader("../tests/mock/reader_test_bit.bin");
    for (size_t i = 0; i < expected_data.size(); ++i) {
        ASSERT_FALSE(reader.IsEOF());
        auto read_byte = reader.Read(1);
        ASSERT_EQ(read_byte, expected_data[i]);
    }
    ASSERT_TRUE(reader.IsEOF());
}

TEST(Reader, ReadByMultipleBits) {
    const std::vector<uint16_t> expected_data = {
        257, 385, 1
    };
    const std::vector<uint8_t> data_sizes = {
        9, 9, 6
    };
    ASSERT_TRUE(data_sizes.size() == expected_data.size());
    Reader reader("../tests/mock/reader_test_multiple.bin");
    for (size_t i = 0; i < expected_data.size(); ++i) {
        ASSERT_FALSE(reader.IsEOF());
        auto read_byte = reader.Read(data_sizes[i]);
        ASSERT_EQ(read_byte, expected_data[i]);
    }
    ASSERT_TRUE(reader.IsEOF());
}

TEST(Reader, ReadBytes) {
    const std::vector<uint16_t> expected_data = {
        1, 3, 7
    };
    const std::vector<uint8_t> data_sizes = {
        8, 8, 8
    };
    ASSERT_TRUE(data_sizes.size() == expected_data.size());
    Reader reader("../tests/mock/reader_test_multiple.bin");
    for (size_t i = 0; i < expected_data.size(); ++i) {
        ASSERT_FALSE(reader.IsEOF());
        auto read_byte = reader.Read(data_sizes[i]);
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
        writer.Write(i < 4, 1);
    }
    writer.Finish();
    std::vector<char> p1 = ReadBytes("writer_test1.bin");
    std::vector<char> p2 = ReadBytes("../tests/mock/writer_test_bit.bin");
    size_t len1 = p1.size();
    size_t len2 = p2.size();
    ASSERT_EQ(len1, 1);
    ASSERT_EQ(len2, 1);
    unsigned char b1 = p1[0];
    unsigned char b2 = p2[0];
    ASSERT_EQ(b1, 15);
    ASSERT_EQ(b2, 15);
    ASSERT_EQ(b1, b2);
}

TEST(Writer, Write9BitsTwice) {
    Writer writer("writer_test2.bin");
    writer.Write(257, 9);
    writer.Write(385, 9);
    writer.Write(1, 6);
    writer.Finish();
    std::vector<char> p1 = ReadBytes("writer_test2.bin");
    std::vector<char> p2 = ReadBytes("../tests/mock/writer_test_multiple.bin"); 
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
    wr.Write(4, 9);
    wr.Write(97, 9);
    wr.Write(258, 9);
    wr.Write(256, 9);
    wr.Write(257, 9);
    wr.Write(1, 9);
    wr.Write(1, 9);
    wr.Write(2, 9);
    wr.Write(0, 1);
    wr.Write(0b011, 3); // 110 in big endian
    wr.Write(0, 1);
    wr.Write(0, 1);
    wr.Write(0, 1);
    wr.Write(0, 1);
    wr.Write(0, 1);
    wr.Write(0b01, 2); // 10 in big endian
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

