#include "cachealgs.h"
#include <algorithm>
#include <QDebug>
static QMap<CacheAlgorithms::CacheAlgorithms, AReplacementFactory*> factory = {
    {CacheAlgorithms::LRU, new LRUFactory(0)}
};

#include <deque>
RecentReplace::RecentReplace(quint16 size, SelectFunction element_select)
: last_access(size, 0), count(0), element_select(element_select)
{

}

RecentReplace::~RecentReplace() = default;

void RecentReplace::reference(quint16 index)
{
    last_access[index] = count++;
}

quint16 RecentReplace::evict()
{
    auto index = element_select(last_access.begin(), last_access.end()) - last_access.begin();
    last_access[index] = count++;
    return index;
}

quint16 RecentReplace::supports_evicition_lookahead() const
{
    return 1;
}

QVector<quint16> RecentReplace::eviction_loohahead(quint16 /*count*/) const
{
    auto index = element_select(last_access.cbegin(), last_access.cend()) - last_access.cbegin();
    return QVector<quint16>{static_cast<quint16>(index)};
}

void RecentReplace::clear()
{
    count = 0;
    for(auto& index: last_access) {
        index = 0;
    }
}

LRUReplace::LRUReplace(quint16 size):
    RecentReplace(size, RecentReplace::SelectFunction(std::min_element<RecentReplace::iterator>))
{

}

QString LRUReplace::get_algorithm_name() const
{
    return LRUFactory::algorithm();
}

MRUReplace::MRUReplace(quint16 size):
    RecentReplace(size, RecentReplace::SelectFunction(std::max_element<RecentReplace::iterator>))
{

}

QString MRUReplace::get_algorithm_name() const
{
    return MRUFactory::algorithm();
}


LRUFactory::LRUFactory(quint16 associativity): AReplacementFactory(associativity)
{

}

QSharedPointer<AReplacementPolicy> LRUFactory::create_policy()
{
    return QSharedPointer<LRUReplace>::create(get_associativity());
}

const QString LRUFactory::algorithm()
{
    QMetaObject meta = CacheAlgorithms::staticMetaObject;
    QMetaEnum metaEnum = meta.enumerator(meta.indexOfEnumerator("CacheAlgorithms"));
    return QString(metaEnum.key(CacheAlgorithms::LRU));
}

CacheAlgorithms::CacheAlgorithms LRUFactory::algorithm_enum()
{
    return CacheAlgorithms::LRU;
}

MRUFactory::MRUFactory(quint16 associativity): AReplacementFactory(associativity)
{

}

QSharedPointer<AReplacementPolicy> MRUFactory::create_policy()
{
    return QSharedPointer<MRUReplace>::create(get_associativity());
}

const QString MRUFactory::algorithm()
{
    QMetaObject meta = CacheAlgorithms::staticMetaObject;
    QMetaEnum metaEnum = meta.enumerator(meta.indexOfEnumerator("CacheAlgorithms"));
    return QString(metaEnum.key(CacheAlgorithms::MRU));
}

CacheAlgorithms::CacheAlgorithms MRUFactory::algorithm_enum()
{
    return CacheAlgorithms::MRU;
}

/*
 * Frequency Replacement
 */
FrequencyReplace::FrequencyReplace(quint16 size, SelectFunction element_select)
: access_count(size, 0), element_select(element_select)
{

}

FrequencyReplace::~FrequencyReplace() = default;

void FrequencyReplace::reference(quint16 index)
{
    access_count[index]++;
}

quint16 FrequencyReplace::evict()
{
    quint16 location;
    // If an entry hasn't been referenced, then it is not present.
    if(auto zero_pos = std::find(access_count.cbegin(), access_count.cend(), 0);
            zero_pos != access_count.cend()) {
        location = zero_pos - access_count.cbegin();
    }
    // Otherwise we must evict a real cache entry instead of a not-present entry.
    else {
        location = element_select(access_count.begin(), access_count.end()) - access_count.begin();
    }
    auto init_value = *std::min(access_count.begin(), access_count.end());
    access_count[location] = init_value;
    return location;
}

quint16 FrequencyReplace::supports_evicition_lookahead() const
{
    return 1;
}

QVector<quint16> FrequencyReplace::eviction_loohahead(quint16 count) const
{
    quint16 location;
    // If an entry hasn't been referenced, then it is not present.
    if(auto zero_pos = std::find(access_count.cbegin(), access_count.cend(), 0);
            zero_pos != access_count.cend()) {
        location = zero_pos - access_count.cbegin();
    }
    // Otherwise we must evict a real cache entry instead of a not-present entry.
    else {
        location = element_select(access_count.cbegin(), access_count.cend()) - access_count.cbegin();
    }
    return QVector<quint16>{static_cast<quint16>(location)};
}

void FrequencyReplace::clear()
{
    for(auto& index: access_count) {
        index = 0;
    }
}

LFUReplace::LFUReplace(quint16 size):
    FrequencyReplace(size, FrequencyReplace::SelectFunction(std::min_element<FrequencyReplace::iterator>))
{

}

QString LFUReplace::get_algorithm_name() const
{
    return LFUFactory::algorithm();
}

MFUReplace::MFUReplace(quint16 size):
    FrequencyReplace(size, FrequencyReplace::SelectFunction(std::max_element<FrequencyReplace::iterator>))
{

}

QString MFUReplace::get_algorithm_name() const
{
    return MFUFactory::algorithm();
}

LFUFactory::LFUFactory(quint16 associativity): AReplacementFactory(associativity)
{

}

QSharedPointer<AReplacementPolicy> LFUFactory::create_policy()
{
    return QSharedPointer<LFUReplace>::create(get_associativity());
}

const QString LFUFactory::algorithm()
{
    QMetaObject meta = CacheAlgorithms::staticMetaObject;
    QMetaEnum metaEnum = meta.enumerator(meta.indexOfEnumerator("CacheAlgorithms"));
    return QString(metaEnum.key(CacheAlgorithms::LFU));
}

CacheAlgorithms::CacheAlgorithms LFUFactory::algorithm_enum()
{
    return CacheAlgorithms::LFU;
}

MFUFactory::MFUFactory(quint16 associativity): AReplacementFactory(associativity)
{

}

QSharedPointer<AReplacementPolicy> MFUFactory::create_policy()
{
    return QSharedPointer<MFUReplace>::create(get_associativity());
}

const QString MFUFactory::algorithm()
{
    QMetaObject meta = CacheAlgorithms::staticMetaObject;
    QMetaEnum metaEnum = meta.enumerator(meta.indexOfEnumerator("CacheAlgorithms"));
    return QString(metaEnum.key(CacheAlgorithms::MFU));
}

CacheAlgorithms::CacheAlgorithms MFUFactory::algorithm_enum()
{
    return CacheAlgorithms::MFU;
}

/*
 * Order Replacement
 */

FIFOReplace::FIFOReplace(quint16 size)
: next_victim(0), size(size)
{

}

void FIFOReplace::reference(quint16 index)
{
    // NOP
}

quint16 FIFOReplace::evict()
{
    auto victim = next_victim;
    next_victim = (next_victim + 1) % size;
    return victim;
}

quint16 FIFOReplace::supports_evicition_lookahead() const
{
    return size;
}

QVector<quint16> FIFOReplace::eviction_loohahead(quint16 count) const
{
    auto items = next_victim;

    if(count>size) count = size;
    auto retVal = QVector<quint16>();
    retVal.resize(count);

    for(int it=0; it<count; it++)
    {
        retVal[it] = items;
        items = (items+1) % size;
    }
    return retVal;
}

void FIFOReplace::clear()
{
    next_victim = 0;
}

QString FIFOReplace::get_algorithm_name() const
{
    return FIFOFactory::algorithm();
}


FIFOFactory::FIFOFactory(quint16 associativity): AReplacementFactory(associativity)
{

}

QSharedPointer<AReplacementPolicy> FIFOFactory::create_policy()
{
    return QSharedPointer<FIFOReplace>::create(get_associativity());
}

const QString FIFOFactory::algorithm()
{
    QMetaObject meta = CacheAlgorithms::staticMetaObject;
    QMetaEnum metaEnum = meta.enumerator(meta.indexOfEnumerator("CacheAlgorithms"));
    return QString(metaEnum.key(CacheAlgorithms::FIFO));
}

CacheAlgorithms::CacheAlgorithms FIFOFactory::algorithm_enum()
{
    return CacheAlgorithms::FIFO;
}

/*
 * Random Replacement Policies
 */
RandomReplace::RandomReplace(std::function<quint16 ()> get_random):
    get_random(get_random)
{
}

void RandomReplace::reference(quint16 /*index*/)
{
    // No-operation
}

quint16 RandomReplace::evict()
{
    if(has_next_random) return next_random;
    else return get_random();
}

quint16 RandomReplace::supports_evicition_lookahead() const
{
    return  1;
}

QVector<quint16> RandomReplace::eviction_loohahead(quint16 count) const
{
    if(!has_next_random) {
        has_next_random = true;
        next_random = get_random();
    }
    QVector<quint16>{next_random};
}

void RandomReplace::clear()
{
    // No-operation
}

QString RandomReplace::get_algorithm_name() const {
    return RandomFactory::algorithm();
}

RandomFactory::RandomFactory(quint16 associativity): AReplacementFactory(associativity)
{
    distribution = std::uniform_int_distribution<quint16>(0, associativity -1);
    random_function = std::function<quint16()>([&](){return distribution(generator);});
}

QSharedPointer<AReplacementPolicy> RandomFactory::create_policy()
{
    return QSharedPointer<RandomReplace>::create(random_function);
}

QString RandomFactory::get_algorithm_name() const
{
    return algorithm();
}

const QString RandomFactory::algorithm()
{
    QMetaObject meta = CacheAlgorithms::staticMetaObject;
    QMetaEnum metaEnum = meta.enumerator(meta.indexOfEnumerator("CacheAlgorithms"));
    return QString(metaEnum.key(CacheAlgorithms::Random));
}

CacheAlgorithms::CacheAlgorithms RandomFactory::algorithm_enum()
{
    return CacheAlgorithms::Random;
}
