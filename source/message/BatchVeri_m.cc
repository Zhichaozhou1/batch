//
// Generated file, do not edit! Created by nedtool 5.6 from veins/modules/messages/BatchVeri.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include "BatchVeri_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

namespace {
template <class T> inline
typename std::enable_if<std::is_polymorphic<T>::value && std::is_base_of<omnetpp::cObject,T>::value, void *>::type
toVoidPtr(T* t)
{
    return (void *)(static_cast<const omnetpp::cObject *>(t));
}

template <class T> inline
typename std::enable_if<std::is_polymorphic<T>::value && !std::is_base_of<omnetpp::cObject,T>::value, void *>::type
toVoidPtr(T* t)
{
    return (void *)dynamic_cast<const void *>(t);
}

template <class T> inline
typename std::enable_if<!std::is_polymorphic<T>::value, void *>::type
toVoidPtr(T* t)
{
    return (void *)static_cast<const void *>(t);
}

}


// forward
template<typename T, typename A>
std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec);

// Template rule to generate operator<< for shared_ptr<T>
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const std::shared_ptr<T>& t) { return out << t.get(); }

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// operator<< for std::vector<T>
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');

    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

MsgKeyPair::MsgKeyPair()
{
    this->senderId = 0;
    this->keyId = 0;
}

void __doPacking(omnetpp::cCommBuffer *b, const MsgKeyPair& a)
{
    doParsimPacking(b,a.senderId);
    doParsimPacking(b,a.keyId);
}

void __doUnpacking(omnetpp::cCommBuffer *b, MsgKeyPair& a)
{
    doParsimUnpacking(b,a.senderId);
    doParsimUnpacking(b,a.keyId);
}

class MsgKeyPairDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
    enum FieldConstants {
        FIELD_senderId,
        FIELD_keyId,
    };
  public:
    MsgKeyPairDescriptor();
    virtual ~MsgKeyPairDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(MsgKeyPairDescriptor)

MsgKeyPairDescriptor::MsgKeyPairDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(MsgKeyPair)), "")
{
    propertynames = nullptr;
}

MsgKeyPairDescriptor::~MsgKeyPairDescriptor()
{
    delete[] propertynames;
}

bool MsgKeyPairDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<MsgKeyPair *>(obj)!=nullptr;
}

const char **MsgKeyPairDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *MsgKeyPairDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int MsgKeyPairDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount() : 2;
}

unsigned int MsgKeyPairDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_senderId
        FD_ISEDITABLE,    // FIELD_keyId
    };
    return (field >= 0 && field < 2) ? fieldTypeFlags[field] : 0;
}

const char *MsgKeyPairDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "senderId",
        "keyId",
    };
    return (field >= 0 && field < 2) ? fieldNames[field] : nullptr;
}

int MsgKeyPairDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0] == 's' && strcmp(fieldName, "senderId") == 0) return base+0;
    if (fieldName[0] == 'k' && strcmp(fieldName, "keyId") == 0) return base+1;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *MsgKeyPairDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_senderId
        "int",    // FIELD_keyId
    };
    return (field >= 0 && field < 2) ? fieldTypeStrings[field] : nullptr;
}

const char **MsgKeyPairDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *MsgKeyPairDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int MsgKeyPairDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    MsgKeyPair *pp = (MsgKeyPair *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *MsgKeyPairDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    MsgKeyPair *pp = (MsgKeyPair *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string MsgKeyPairDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    MsgKeyPair *pp = (MsgKeyPair *)object; (void)pp;
    switch (field) {
        case FIELD_senderId: return long2string(pp->senderId);
        case FIELD_keyId: return long2string(pp->keyId);
        default: return "";
    }
}

bool MsgKeyPairDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    MsgKeyPair *pp = (MsgKeyPair *)object; (void)pp;
    switch (field) {
        case FIELD_senderId: pp->senderId = string2long(value); return true;
        case FIELD_keyId: pp->keyId = string2long(value); return true;
        default: return false;
    }
}

const char *MsgKeyPairDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *MsgKeyPairDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    MsgKeyPair *pp = (MsgKeyPair *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(BatchVeri)

BatchVeri::BatchVeri(const char *name, short kind) : ::veins::BaseFrame1609_4(name, kind)
{
}

BatchVeri::BatchVeri(const BatchVeri& other) : ::veins::BaseFrame1609_4(other)
{
    copy(other);
}

BatchVeri::~BatchVeri()
{
    delete [] this->pairs;
}

BatchVeri& BatchVeri::operator=(const BatchVeri& other)
{
    if (this == &other) return *this;
    ::veins::BaseFrame1609_4::operator=(other);
    copy(other);
    return *this;
}

void BatchVeri::copy(const BatchVeri& other)
{
    this->kind = other.kind;
    this->senderId = other.senderId;
    this->timestamp = other.timestamp;
    this->beaconId = other.beaconId;
    this->KeyId = other.KeyId;
    this->sigVrfd = other.sigVrfd;
    this->PCVrfd = other.PCVrfd;
    this->MsgId = other.MsgId;
    this->hashchainVrfd = other.hashchainVrfd;
    this->puzzleVrfd = other.puzzleVrfd;
    delete [] this->pairs;
    this->pairs = (other.pairs_arraysize==0) ? nullptr : new MsgKeyPair[other.pairs_arraysize];
    pairs_arraysize = other.pairs_arraysize;
    for (size_t i = 0; i < pairs_arraysize; i++) {
        this->pairs[i] = other.pairs[i];
    }
}

void BatchVeri::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::veins::BaseFrame1609_4::parsimPack(b);
    doParsimPacking(b,this->kind);
    doParsimPacking(b,this->senderId);
    doParsimPacking(b,this->timestamp);
    doParsimPacking(b,this->beaconId);
    doParsimPacking(b,this->KeyId);
    doParsimPacking(b,this->sigVrfd);
    doParsimPacking(b,this->PCVrfd);
    doParsimPacking(b,this->MsgId);
    doParsimPacking(b,this->hashchainVrfd);
    doParsimPacking(b,this->puzzleVrfd);
    b->pack(pairs_arraysize);
    doParsimArrayPacking(b,this->pairs,pairs_arraysize);
}

void BatchVeri::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::veins::BaseFrame1609_4::parsimUnpack(b);
    doParsimUnpacking(b,this->kind);
    doParsimUnpacking(b,this->senderId);
    doParsimUnpacking(b,this->timestamp);
    doParsimUnpacking(b,this->beaconId);
    doParsimUnpacking(b,this->KeyId);
    doParsimUnpacking(b,this->sigVrfd);
    doParsimUnpacking(b,this->PCVrfd);
    doParsimUnpacking(b,this->MsgId);
    doParsimUnpacking(b,this->hashchainVrfd);
    doParsimUnpacking(b,this->puzzleVrfd);
    delete [] this->pairs;
    b->unpack(pairs_arraysize);
    if (pairs_arraysize == 0) {
        this->pairs = nullptr;
    } else {
        this->pairs = new MsgKeyPair[pairs_arraysize];
        doParsimArrayUnpacking(b,this->pairs,pairs_arraysize);
    }
}

int BatchVeri::getKind() const
{
    return this->kind;
}

void BatchVeri::setKind(int kind)
{
    this->kind = kind;
}

int BatchVeri::getSenderId() const
{
    return this->senderId;
}

void BatchVeri::setSenderId(int senderId)
{
    this->senderId = senderId;
}

omnetpp::simtime_t BatchVeri::getTimestamp() const
{
    return this->timestamp;
}

void BatchVeri::setTimestamp(omnetpp::simtime_t timestamp)
{
    this->timestamp = timestamp;
}

int BatchVeri::getBeaconId() const
{
    return this->beaconId;
}

void BatchVeri::setBeaconId(int beaconId)
{
    this->beaconId = beaconId;
}

int BatchVeri::getKeyId() const
{
    return this->KeyId;
}

void BatchVeri::setKeyId(int KeyId)
{
    this->KeyId = KeyId;
}

bool BatchVeri::getSigVrfd() const
{
    return this->sigVrfd;
}

void BatchVeri::setSigVrfd(bool sigVrfd)
{
    this->sigVrfd = sigVrfd;
}

bool BatchVeri::getPCVrfd() const
{
    return this->PCVrfd;
}

void BatchVeri::setPCVrfd(bool PCVrfd)
{
    this->PCVrfd = PCVrfd;
}

const char * BatchVeri::getMsgId() const
{
    return this->MsgId.c_str();
}

void BatchVeri::setMsgId(const char * MsgId)
{
    this->MsgId = MsgId;
}

bool BatchVeri::getHashchainVrfd() const
{
    return this->hashchainVrfd;
}

void BatchVeri::setHashchainVrfd(bool hashchainVrfd)
{
    this->hashchainVrfd = hashchainVrfd;
}

bool BatchVeri::getPuzzleVrfd() const
{
    return this->puzzleVrfd;
}

void BatchVeri::setPuzzleVrfd(bool puzzleVrfd)
{
    this->puzzleVrfd = puzzleVrfd;
}

size_t BatchVeri::getPairsArraySize() const
{
    return pairs_arraysize;
}

const MsgKeyPair& BatchVeri::getPairs(size_t k) const
{
    if (k >= pairs_arraysize) throw omnetpp::cRuntimeError("Array of size pairs_arraysize indexed by %lu", (unsigned long)k);
    return this->pairs[k];
}

void BatchVeri::setPairsArraySize(size_t newSize)
{
    MsgKeyPair *pairs2 = (newSize==0) ? nullptr : new MsgKeyPair[newSize];
    size_t minSize = pairs_arraysize < newSize ? pairs_arraysize : newSize;
    for (size_t i = 0; i < minSize; i++)
        pairs2[i] = this->pairs[i];
    delete [] this->pairs;
    this->pairs = pairs2;
    pairs_arraysize = newSize;
}

void BatchVeri::setPairs(size_t k, const MsgKeyPair& pairs)
{
    if (k >= pairs_arraysize) throw omnetpp::cRuntimeError("Array of size  indexed by %lu", (unsigned long)k);
    this->pairs[k] = pairs;
}

void BatchVeri::insertPairs(size_t k, const MsgKeyPair& pairs)
{
    if (k > pairs_arraysize) throw omnetpp::cRuntimeError("Array of size  indexed by %lu", (unsigned long)k);
    size_t newSize = pairs_arraysize + 1;
    MsgKeyPair *pairs2 = new MsgKeyPair[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        pairs2[i] = this->pairs[i];
    pairs2[k] = pairs;
    for (i = k + 1; i < newSize; i++)
        pairs2[i] = this->pairs[i-1];
    delete [] this->pairs;
    this->pairs = pairs2;
    pairs_arraysize = newSize;
}

void BatchVeri::insertPairs(const MsgKeyPair& pairs)
{
    insertPairs(pairs_arraysize, pairs);
}

void BatchVeri::erasePairs(size_t k)
{
    if (k >= pairs_arraysize) throw omnetpp::cRuntimeError("Array of size  indexed by %lu", (unsigned long)k);
    size_t newSize = pairs_arraysize - 1;
    MsgKeyPair *pairs2 = (newSize == 0) ? nullptr : new MsgKeyPair[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        pairs2[i] = this->pairs[i];
    for (i = k; i < newSize; i++)
        pairs2[i] = this->pairs[i+1];
    delete [] this->pairs;
    this->pairs = pairs2;
    pairs_arraysize = newSize;
}

class BatchVeriDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
    enum FieldConstants {
        FIELD_kind,
        FIELD_senderId,
        FIELD_timestamp,
        FIELD_beaconId,
        FIELD_KeyId,
        FIELD_sigVrfd,
        FIELD_PCVrfd,
        FIELD_MsgId,
        FIELD_hashchainVrfd,
        FIELD_puzzleVrfd,
        FIELD_pairs,
    };
  public:
    BatchVeriDescriptor();
    virtual ~BatchVeriDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(BatchVeriDescriptor)

BatchVeriDescriptor::BatchVeriDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(BatchVeri)), "veins::BaseFrame1609_4")
{
    propertynames = nullptr;
}

BatchVeriDescriptor::~BatchVeriDescriptor()
{
    delete[] propertynames;
}

bool BatchVeriDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<BatchVeri *>(obj)!=nullptr;
}

const char **BatchVeriDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *BatchVeriDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int BatchVeriDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 11+basedesc->getFieldCount() : 11;
}

unsigned int BatchVeriDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_kind
        FD_ISEDITABLE,    // FIELD_senderId
        0,    // FIELD_timestamp
        FD_ISEDITABLE,    // FIELD_beaconId
        FD_ISEDITABLE,    // FIELD_KeyId
        FD_ISEDITABLE,    // FIELD_sigVrfd
        FD_ISEDITABLE,    // FIELD_PCVrfd
        FD_ISEDITABLE,    // FIELD_MsgId
        FD_ISEDITABLE,    // FIELD_hashchainVrfd
        FD_ISEDITABLE,    // FIELD_puzzleVrfd
        FD_ISARRAY | FD_ISCOMPOUND,    // FIELD_pairs
    };
    return (field >= 0 && field < 11) ? fieldTypeFlags[field] : 0;
}

const char *BatchVeriDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "kind",
        "senderId",
        "timestamp",
        "beaconId",
        "KeyId",
        "sigVrfd",
        "PCVrfd",
        "MsgId",
        "hashchainVrfd",
        "puzzleVrfd",
        "pairs",
    };
    return (field >= 0 && field < 11) ? fieldNames[field] : nullptr;
}

int BatchVeriDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0] == 'k' && strcmp(fieldName, "kind") == 0) return base+0;
    if (fieldName[0] == 's' && strcmp(fieldName, "senderId") == 0) return base+1;
    if (fieldName[0] == 't' && strcmp(fieldName, "timestamp") == 0) return base+2;
    if (fieldName[0] == 'b' && strcmp(fieldName, "beaconId") == 0) return base+3;
    if (fieldName[0] == 'K' && strcmp(fieldName, "KeyId") == 0) return base+4;
    if (fieldName[0] == 's' && strcmp(fieldName, "sigVrfd") == 0) return base+5;
    if (fieldName[0] == 'P' && strcmp(fieldName, "PCVrfd") == 0) return base+6;
    if (fieldName[0] == 'M' && strcmp(fieldName, "MsgId") == 0) return base+7;
    if (fieldName[0] == 'h' && strcmp(fieldName, "hashchainVrfd") == 0) return base+8;
    if (fieldName[0] == 'p' && strcmp(fieldName, "puzzleVrfd") == 0) return base+9;
    if (fieldName[0] == 'p' && strcmp(fieldName, "pairs") == 0) return base+10;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *BatchVeriDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_kind
        "int",    // FIELD_senderId
        "omnetpp::simtime_t",    // FIELD_timestamp
        "int",    // FIELD_beaconId
        "int",    // FIELD_KeyId
        "bool",    // FIELD_sigVrfd
        "bool",    // FIELD_PCVrfd
        "string",    // FIELD_MsgId
        "bool",    // FIELD_hashchainVrfd
        "bool",    // FIELD_puzzleVrfd
        "MsgKeyPair",    // FIELD_pairs
    };
    return (field >= 0 && field < 11) ? fieldTypeStrings[field] : nullptr;
}

const char **BatchVeriDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *BatchVeriDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int BatchVeriDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    BatchVeri *pp = (BatchVeri *)object; (void)pp;
    switch (field) {
        case FIELD_pairs: return pp->getPairsArraySize();
        default: return 0;
    }
}

const char *BatchVeriDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    BatchVeri *pp = (BatchVeri *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string BatchVeriDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    BatchVeri *pp = (BatchVeri *)object; (void)pp;
    switch (field) {
        case FIELD_kind: return long2string(pp->getKind());
        case FIELD_senderId: return long2string(pp->getSenderId());
        case FIELD_timestamp: return simtime2string(pp->getTimestamp());
        case FIELD_beaconId: return long2string(pp->getBeaconId());
        case FIELD_KeyId: return long2string(pp->getKeyId());
        case FIELD_sigVrfd: return bool2string(pp->getSigVrfd());
        case FIELD_PCVrfd: return bool2string(pp->getPCVrfd());
        case FIELD_MsgId: return oppstring2string(pp->getMsgId());
        case FIELD_hashchainVrfd: return bool2string(pp->getHashchainVrfd());
        case FIELD_puzzleVrfd: return bool2string(pp->getPuzzleVrfd());
        case FIELD_pairs: {std::stringstream out; out << pp->getPairs(i); return out.str();}
        default: return "";
    }
}

bool BatchVeriDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    BatchVeri *pp = (BatchVeri *)object; (void)pp;
    switch (field) {
        case FIELD_kind: pp->setKind(string2long(value)); return true;
        case FIELD_senderId: pp->setSenderId(string2long(value)); return true;
        case FIELD_beaconId: pp->setBeaconId(string2long(value)); return true;
        case FIELD_KeyId: pp->setKeyId(string2long(value)); return true;
        case FIELD_sigVrfd: pp->setSigVrfd(string2bool(value)); return true;
        case FIELD_PCVrfd: pp->setPCVrfd(string2bool(value)); return true;
        case FIELD_MsgId: pp->setMsgId((value)); return true;
        case FIELD_hashchainVrfd: pp->setHashchainVrfd(string2bool(value)); return true;
        case FIELD_puzzleVrfd: pp->setPuzzleVrfd(string2bool(value)); return true;
        default: return false;
    }
}

const char *BatchVeriDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case FIELD_pairs: return omnetpp::opp_typename(typeid(MsgKeyPair));
        default: return nullptr;
    };
}

void *BatchVeriDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    BatchVeri *pp = (BatchVeri *)object; (void)pp;
    switch (field) {
        case FIELD_pairs: return toVoidPtr(&pp->getPairs(i)); break;
        default: return nullptr;
    }
}

