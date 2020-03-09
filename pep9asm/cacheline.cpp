#include "cacheline.h"
#include "cachealgs.h"
#include <QDebug>

CacheLine::CacheLine(): entries(0, CacheEntry()), replacement_policy(nullptr)
{

}

CacheLine::CacheLine(quint8 associativity, QSharedPointer<AReplacementPolicy> replacement_policy):
    entries(associativity, CacheEntry()), replacement_policy(replacement_policy)
{

}

bool CacheLine::contains_index(quint16 index)
{
    for(auto entry : entries) {
        if(entry.is_present && entry.index == index) return true;
    }
    return false;
}

void CacheLine::update(quint16 index)
{
    for(int it = 0; it < entries.size(); it++) {
        auto& entry = entries[it];
        if(entry.is_present && entry.index == index) {
            // TODO: Perform cache replacement policy update on present entry.
            replacement_policy->reference(it);
            entry.hit_count++;
        }
    }
}

void CacheLine::insert(quint16 index)
{
    // TODO: get replacement index for CRP.
    quint16 evict_index = replacement_policy->evict();
    qDebug().noquote() << QString("Evicting index %1 for index %2").arg(entries[evict_index].index).arg(index);
    entries[evict_index].is_present = true;
    entries[evict_index].index = index;
    entries[evict_index].hit_count = 1;
}

void CacheLine::clear()
{
    replacement_policy->clear();
    for(auto& entry : entries) {
        entry.is_present = false;
        entry.index = 0;
        entry.hit_count = 0;
    }
}

std::optional<const CacheEntry *> CacheLine::get_entry(quint16 position) const
{
    if(position >= (1 << entries.size())) return std::nullopt;
    else return &entries[position];
}

QSharedPointer<const AReplacementPolicy> CacheLine::get_replacement_policy() const
{
    return replacement_policy;
}
