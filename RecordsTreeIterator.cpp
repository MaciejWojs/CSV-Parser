#include "./RecordsTreeIterator.hpp"
#include "recordsTree.hpp"

RecordsTreeIterator::RecordsTreeIterator(RecordsTree* tree, bool end) : tree(tree) {
    if (!tree || end || tree->years.empty()) {
        yearIt = tree ? tree->years.end() : std::map<int, YearNode>::iterator();
        return;
    }

    yearIt = tree->years.begin();
    monthIt = yearIt->second.months.begin();
    dayIt = monthIt->second.days.begin();
    quarterIt = dayIt->second.quarters.begin();
    recordIt = quarterIt->second.records.begin();

    if (recordIt == quarterIt->second.records.end()) {
        findNextValidPosition();
    }
}

void RecordsTreeIterator::findNextValidPosition() {
    while (yearIt != tree->years.end()) {
        while (monthIt != yearIt->second.months.end()) {
            while (dayIt != monthIt->second.days.end()) {
                while (quarterIt != dayIt->second.quarters.end()) {
                    if (recordIt != quarterIt->second.records.end()) {
                        return;
                    }
                    ++quarterIt;
                    if (quarterIt != dayIt->second.quarters.end()) {
                        recordIt = quarterIt->second.records.begin();
                    }
                }
                ++dayIt;
                if (dayIt != monthIt->second.days.end()) {
                    quarterIt = dayIt->second.quarters.begin();
                    recordIt = quarterIt->second.records.begin();
                }
            }
            ++monthIt;
            if (monthIt != yearIt->second.months.end()) {
                dayIt = monthIt->second.days.begin();
                quarterIt = dayIt->second.quarters.begin();
                recordIt = quarterIt->second.records.begin();
            }
        }
        ++yearIt;
        if (yearIt != tree->years.end()) {
            monthIt = yearIt->second.months.begin();
            dayIt = monthIt->second.days.begin();
            quarterIt = dayIt->second.quarters.begin();
            recordIt = quarterIt->second.records.begin();
        }
    }
}

RecordsTreeIterator& RecordsTreeIterator::operator++() {
    if (yearIt == tree->years.end()) return *this;

    ++recordIt;
    findNextValidPosition();
    return *this;
}

RecordsTreeIterator RecordsTreeIterator::operator++(int) {
    RecordsTreeIterator tmp = *this;
    ++(*this);
    return tmp;
}

bool RecordsTreeIterator::operator==(const RecordsTreeIterator& other) const {
    return yearIt == other.yearIt;
}

bool RecordsTreeIterator::operator!=(const RecordsTreeIterator& other) const {
    return !(*this == other);
}

Record& RecordsTreeIterator::operator*() const {
    return *recordIt;
}

Record* RecordsTreeIterator::operator->() const {
    return &(*recordIt);
}