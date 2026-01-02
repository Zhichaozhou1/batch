//
// Generated file, do not edit! Created by nedtool 5.6 from veins/modules/messages/BeaconMessage.msg.
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
#include "BeaconMessage_m.h"

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

Register_Class(BeaconMessage)

BeaconMessage::BeaconMessage(const char *name, short kind) : ::veins::BaseFrame1609_4(name, kind)
{
}

BeaconMessage::BeaconMessage(const BeaconMessage& other) : ::veins::BaseFrame1609_4(other)
{
    copy(other);
}

BeaconMessage::~BeaconMessage()
{
}

BeaconMessage& BeaconMessage::operator=(const BeaconMessage& other)
{
    if (this == &other) return *this;
    ::veins::BaseFrame1609_4::operator=(other);
    copy(other);
    return *this;
}

void BeaconMessage::copy(const BeaconMessage& other)
{
    this->kind = other.kind;
    this->senderId = other.senderId;
    this->geosenderId = other.geosenderId;
    this->beaconId = other.beaconId;
    this->KeyId = other.KeyId;
    this->RSUId = other.RSUId;
    this->geocast = other.geocast;
    this->MsgId = other.MsgId;
    this->timestamp = other.timestamp;
    this->timestamp_ini = other.timestamp_ini;
    this->sigVrfd = other.sigVrfd;
    this->puzzleVrfd = other.puzzleVrfd;
    this->PCVrfd = other.PCVrfd;
    this->hashchainVrfd = other.hashchainVrfd;
    this->braking = other.braking;
    this->puzzle = other.puzzle;
    this->senderPos = other.senderPos;
    this->senderSpeed = other.senderSpeed;
}

void BeaconMessage::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::veins::BaseFrame1609_4::parsimPack(b);
    doParsimPacking(b,this->kind);
    doParsimPacking(b,this->senderId);
    doParsimPacking(b,this->geosenderId);
    doParsimPacking(b,this->beaconId);
    doParsimPacking(b,this->KeyId);
    doParsimPacking(b,this->RSUId);
    doParsimPacking(b,this->geocast);
    doParsimPacking(b,this->MsgId);
    doParsimPacking(b,this->timestamp);
    doParsimPacking(b,this->timestamp_ini);
    doParsimPacking(b,this->sigVrfd);
    doParsimPacking(b,this->puzzleVrfd);
    doParsimPacking(b,this->PCVrfd);
    doParsimPacking(b,this->hashchainVrfd);
    doParsimPacking(b,this->braking);
    doParsimPacking(b,this->puzzle);
    doParsimPacking(b,this->senderPos);
    doParsimPacking(b,this->senderSpeed);
}

void BeaconMessage::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::veins::BaseFrame1609_4::parsimUnpack(b);
    doParsimUnpacking(b,this->kind);
    doParsimUnpacking(b,this->senderId);
    doParsimUnpacking(b,this->geosenderId);
    doParsimUnpacking(b,this->beaconId);
    doParsimUnpacking(b,this->KeyId);
    doParsimUnpacking(b,this->RSUId);
    doParsimUnpacking(b,this->geocast);
    doParsimUnpacking(b,this->MsgId);
    doParsimUnpacking(b,this->timestamp);
    doParsimUnpacking(b,this->timestamp_ini);
    doParsimUnpacking(b,this->sigVrfd);
    doParsimUnpacking(b,this->puzzleVrfd);
    doParsimUnpacking(b,this->PCVrfd);
    doParsimUnpacking(b,this->hashchainVrfd);
    doParsimUnpacking(b,this->braking);
    doParsimUnpacking(b,this->puzzle);
    doParsimUnpacking(b,this->senderPos);
    doParsimUnpacking(b,this->senderSpeed);
}

int BeaconMessage::getKind() const
{
    return this->kind;
}

void BeaconMessage::setKind(int kind)
{
    this->kind = kind;
}

int BeaconMessage::getSenderId() const
{
    return this->senderId;
}

void BeaconMessage::setSenderId(int senderId)
{
    this->senderId = senderId;
}

int BeaconMessage::getGeosenderId() const
{
    return this->geosenderId;
}

void BeaconMessage::setGeosenderId(int geosenderId)
{
    this->geosenderId = geosenderId;
}

int BeaconMessage::getBeaconId() const
{
    return this->beaconId;
}

void BeaconMessage::setBeaconId(int beaconId)
{
    this->beaconId = beaconId;
}

int BeaconMessage::getKeyId() const
{
    return this->KeyId;
}

void BeaconMessage::setKeyId(int KeyId)
{
    this->KeyId = KeyId;
}

int BeaconMessage::getRSUId() const
{
    return this->RSUId;
}

void BeaconMessage::setRSUId(int RSUId)
{
    this->RSUId = RSUId;
}

int BeaconMessage::getGeocast() const
{
    return this->geocast;
}

void BeaconMessage::setGeocast(int geocast)
{
    this->geocast = geocast;
}

const char * BeaconMessage::getMsgId() const
{
    return this->MsgId.c_str();
}

void BeaconMessage::setMsgId(const char * MsgId)
{
    this->MsgId = MsgId;
}

omnetpp::simtime_t BeaconMessage::getTimestamp() const
{
    return this->timestamp;
}

void BeaconMessage::setTimestamp(omnetpp::simtime_t timestamp)
{
    this->timestamp = timestamp;
}

omnetpp::simtime_t BeaconMessage::getTimestamp_ini() const
{
    return this->timestamp_ini;
}

void BeaconMessage::setTimestamp_ini(omnetpp::simtime_t timestamp_ini)
{
    this->timestamp_ini = timestamp_ini;
}

bool BeaconMessage::getSigVrfd() const
{
    return this->sigVrfd;
}

void BeaconMessage::setSigVrfd(bool sigVrfd)
{
    this->sigVrfd = sigVrfd;
}

bool BeaconMessage::getPuzzleVrfd() const
{
    return this->puzzleVrfd;
}

void BeaconMessage::setPuzzleVrfd(bool puzzleVrfd)
{
    this->puzzleVrfd = puzzleVrfd;
}

bool BeaconMessage::getPCVrfd() const
{
    return this->PCVrfd;
}

void BeaconMessage::setPCVrfd(bool PCVrfd)
{
    this->PCVrfd = PCVrfd;
}

bool BeaconMessage::getHashchainVrfd() const
{
    return this->hashchainVrfd;
}

void BeaconMessage::setHashchainVrfd(bool hashchainVrfd)
{
    this->hashchainVrfd = hashchainVrfd;
}

bool BeaconMessage::getBraking() const
{
    return this->braking;
}

void BeaconMessage::setBraking(bool braking)
{
    this->braking = braking;
}

const char * BeaconMessage::getPuzzle() const
{
    return this->puzzle.c_str();
}

void BeaconMessage::setPuzzle(const char * puzzle)
{
    this->puzzle = puzzle;
}

const veins::Coord& BeaconMessage::getSenderPos() const
{
    return this->senderPos;
}

void BeaconMessage::setSenderPos(const veins::Coord& senderPos)
{
    this->senderPos = senderPos;
}

const veins::Coord& BeaconMessage::getSenderSpeed() const
{
    return this->senderSpeed;
}

void BeaconMessage::setSenderSpeed(const veins::Coord& senderSpeed)
{
    this->senderSpeed = senderSpeed;
}

class BeaconMessageDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
    enum FieldConstants {
        FIELD_kind,
        FIELD_senderId,
        FIELD_geosenderId,
        FIELD_beaconId,
        FIELD_KeyId,
        FIELD_RSUId,
        FIELD_geocast,
        FIELD_MsgId,
        FIELD_timestamp,
        FIELD_timestamp_ini,
        FIELD_sigVrfd,
        FIELD_puzzleVrfd,
        FIELD_PCVrfd,
        FIELD_hashchainVrfd,
        FIELD_braking,
        FIELD_puzzle,
        FIELD_senderPos,
        FIELD_senderSpeed,
    };
  public:
    BeaconMessageDescriptor();
    virtual ~BeaconMessageDescriptor();

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

Register_ClassDescriptor(BeaconMessageDescriptor)

BeaconMessageDescriptor::BeaconMessageDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(BeaconMessage)), "veins::BaseFrame1609_4")
{
    propertynames = nullptr;
}

BeaconMessageDescriptor::~BeaconMessageDescriptor()
{
    delete[] propertynames;
}

bool BeaconMessageDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<BeaconMessage *>(obj)!=nullptr;
}

const char **BeaconMessageDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *BeaconMessageDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int BeaconMessageDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 18+basedesc->getFieldCount() : 18;
}

unsigned int BeaconMessageDescriptor::getFieldTypeFlags(int field) const
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
        FD_ISEDITABLE,    // FIELD_geosenderId
        FD_ISEDITABLE,    // FIELD_beaconId
        FD_ISEDITABLE,    // FIELD_KeyId
        FD_ISEDITABLE,    // FIELD_RSUId
        FD_ISEDITABLE,    // FIELD_geocast
        FD_ISEDITABLE,    // FIELD_MsgId
        0,    // FIELD_timestamp
        0,    // FIELD_timestamp_ini
        FD_ISEDITABLE,    // FIELD_sigVrfd
        FD_ISEDITABLE,    // FIELD_puzzleVrfd
        FD_ISEDITABLE,    // FIELD_PCVrfd
        FD_ISEDITABLE,    // FIELD_hashchainVrfd
        FD_ISEDITABLE,    // FIELD_braking
        FD_ISEDITABLE,    // FIELD_puzzle
        0,    // FIELD_senderPos
        0,    // FIELD_senderSpeed
    };
    return (field >= 0 && field < 18) ? fieldTypeFlags[field] : 0;
}

const char *BeaconMessageDescriptor::getFieldName(int field) const
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
        "geosenderId",
        "beaconId",
        "KeyId",
        "RSUId",
        "geocast",
        "MsgId",
        "timestamp",
        "timestamp_ini",
        "sigVrfd",
        "puzzleVrfd",
        "PCVrfd",
        "hashchainVrfd",
        "braking",
        "puzzle",
        "senderPos",
        "senderSpeed",
    };
    return (field >= 0 && field < 18) ? fieldNames[field] : nullptr;
}

int BeaconMessageDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0] == 'k' && strcmp(fieldName, "kind") == 0) return base+0;
    if (fieldName[0] == 's' && strcmp(fieldName, "senderId") == 0) return base+1;
    if (fieldName[0] == 'g' && strcmp(fieldName, "geosenderId") == 0) return base+2;
    if (fieldName[0] == 'b' && strcmp(fieldName, "beaconId") == 0) return base+3;
    if (fieldName[0] == 'K' && strcmp(fieldName, "KeyId") == 0) return base+4;
    if (fieldName[0] == 'R' && strcmp(fieldName, "RSUId") == 0) return base+5;
    if (fieldName[0] == 'g' && strcmp(fieldName, "geocast") == 0) return base+6;
    if (fieldName[0] == 'M' && strcmp(fieldName, "MsgId") == 0) return base+7;
    if (fieldName[0] == 't' && strcmp(fieldName, "timestamp") == 0) return base+8;
    if (fieldName[0] == 't' && strcmp(fieldName, "timestamp_ini") == 0) return base+9;
    if (fieldName[0] == 's' && strcmp(fieldName, "sigVrfd") == 0) return base+10;
    if (fieldName[0] == 'p' && strcmp(fieldName, "puzzleVrfd") == 0) return base+11;
    if (fieldName[0] == 'P' && strcmp(fieldName, "PCVrfd") == 0) return base+12;
    if (fieldName[0] == 'h' && strcmp(fieldName, "hashchainVrfd") == 0) return base+13;
    if (fieldName[0] == 'b' && strcmp(fieldName, "braking") == 0) return base+14;
    if (fieldName[0] == 'p' && strcmp(fieldName, "puzzle") == 0) return base+15;
    if (fieldName[0] == 's' && strcmp(fieldName, "senderPos") == 0) return base+16;
    if (fieldName[0] == 's' && strcmp(fieldName, "senderSpeed") == 0) return base+17;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *BeaconMessageDescriptor::getFieldTypeString(int field) const
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
        "int",    // FIELD_geosenderId
        "int",    // FIELD_beaconId
        "int",    // FIELD_KeyId
        "int",    // FIELD_RSUId
        "int",    // FIELD_geocast
        "string",    // FIELD_MsgId
        "omnetpp::simtime_t",    // FIELD_timestamp
        "omnetpp::simtime_t",    // FIELD_timestamp_ini
        "bool",    // FIELD_sigVrfd
        "bool",    // FIELD_puzzleVrfd
        "bool",    // FIELD_PCVrfd
        "bool",    // FIELD_hashchainVrfd
        "bool",    // FIELD_braking
        "string",    // FIELD_puzzle
        "veins::Coord",    // FIELD_senderPos
        "veins::Coord",    // FIELD_senderSpeed
    };
    return (field >= 0 && field < 18) ? fieldTypeStrings[field] : nullptr;
}

const char **BeaconMessageDescriptor::getFieldPropertyNames(int field) const
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

const char *BeaconMessageDescriptor::getFieldProperty(int field, const char *propertyname) const
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

int BeaconMessageDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    BeaconMessage *pp = (BeaconMessage *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *BeaconMessageDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    BeaconMessage *pp = (BeaconMessage *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string BeaconMessageDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    BeaconMessage *pp = (BeaconMessage *)object; (void)pp;
    switch (field) {
        case FIELD_kind: return long2string(pp->getKind());
        case FIELD_senderId: return long2string(pp->getSenderId());
        case FIELD_geosenderId: return long2string(pp->getGeosenderId());
        case FIELD_beaconId: return long2string(pp->getBeaconId());
        case FIELD_KeyId: return long2string(pp->getKeyId());
        case FIELD_RSUId: return long2string(pp->getRSUId());
        case FIELD_geocast: return long2string(pp->getGeocast());
        case FIELD_MsgId: return oppstring2string(pp->getMsgId());
        case FIELD_timestamp: return simtime2string(pp->getTimestamp());
        case FIELD_timestamp_ini: return simtime2string(pp->getTimestamp_ini());
        case FIELD_sigVrfd: return bool2string(pp->getSigVrfd());
        case FIELD_puzzleVrfd: return bool2string(pp->getPuzzleVrfd());
        case FIELD_PCVrfd: return bool2string(pp->getPCVrfd());
        case FIELD_hashchainVrfd: return bool2string(pp->getHashchainVrfd());
        case FIELD_braking: return bool2string(pp->getBraking());
        case FIELD_puzzle: return oppstring2string(pp->getPuzzle());
        case FIELD_senderPos: {std::stringstream out; out << pp->getSenderPos(); return out.str();}
        case FIELD_senderSpeed: {std::stringstream out; out << pp->getSenderSpeed(); return out.str();}
        default: return "";
    }
}

bool BeaconMessageDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    BeaconMessage *pp = (BeaconMessage *)object; (void)pp;
    switch (field) {
        case FIELD_kind: pp->setKind(string2long(value)); return true;
        case FIELD_senderId: pp->setSenderId(string2long(value)); return true;
        case FIELD_geosenderId: pp->setGeosenderId(string2long(value)); return true;
        case FIELD_beaconId: pp->setBeaconId(string2long(value)); return true;
        case FIELD_KeyId: pp->setKeyId(string2long(value)); return true;
        case FIELD_RSUId: pp->setRSUId(string2long(value)); return true;
        case FIELD_geocast: pp->setGeocast(string2long(value)); return true;
        case FIELD_MsgId: pp->setMsgId((value)); return true;
        case FIELD_sigVrfd: pp->setSigVrfd(string2bool(value)); return true;
        case FIELD_puzzleVrfd: pp->setPuzzleVrfd(string2bool(value)); return true;
        case FIELD_PCVrfd: pp->setPCVrfd(string2bool(value)); return true;
        case FIELD_hashchainVrfd: pp->setHashchainVrfd(string2bool(value)); return true;
        case FIELD_braking: pp->setBraking(string2bool(value)); return true;
        case FIELD_puzzle: pp->setPuzzle((value)); return true;
        default: return false;
    }
}

const char *BeaconMessageDescriptor::getFieldStructName(int field) const
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

void *BeaconMessageDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    BeaconMessage *pp = (BeaconMessage *)object; (void)pp;
    switch (field) {
        case FIELD_senderPos: return toVoidPtr(&pp->getSenderPos()); break;
        case FIELD_senderSpeed: return toVoidPtr(&pp->getSenderSpeed()); break;
        default: return nullptr;
    }
}

