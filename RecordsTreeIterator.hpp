#ifndef FA848287_0163_44D1_8165_C791109FEE3E
#define FA848287_0163_44D1_8165_C791109FEE3E

// Include the necessary header for full type definitions
#include "recordsTree.hpp"  // Make sure this is included to define YearNode, MonthNode, etc.
#include <iterator>
#include <map>
#include <vector>
#include "./Record.hpp"

// Forward declarations for types used in the iterator
class RecordsTree;
struct YearNode;
struct MonthNode;
struct DayNode;
struct QuarterNode;

class RecordsTreeIterator {
    public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = Record;
    using reference = Record&;
    using pointer = Record*;
    using difference_type = std::ptrdiff_t;

    RecordsTreeIterator(RecordsTree* tree, bool end = false);

    reference operator*() const;
    pointer operator->() const;

    RecordsTreeIterator& operator++();
    RecordsTreeIterator operator++(int);

    bool operator==(const RecordsTreeIterator& other) const;
    bool operator!=(const RecordsTreeIterator& other) const;

    private:
    RecordsTree* tree;
    std::map<int, YearNode>::iterator yearIt;
    std::map<int, MonthNode>::iterator monthIt;
    std::map<int, DayNode>::iterator dayIt;
    std::map<int, QuarterNode>::iterator quarterIt;
    std::vector<Record>::iterator recordIt;
    pointer current;
    bool isEnd;

    void findNextValidPosition();
};

#endif /* FA848287_0163_44D1_8165_C791109FEE3E */
