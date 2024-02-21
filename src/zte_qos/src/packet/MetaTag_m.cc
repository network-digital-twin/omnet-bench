//
// Generated file, do not edit! Created by opp_msgtool 6.0 from packet/MetaTag.msg.
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
#include <type_traits>
#include "MetaTag_m.h"

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

namespace inet {

Register_Class(MetaTag)

MetaTag::MetaTag() : ::inet::TagBase()
{
}

MetaTag::MetaTag(const MetaTag& other) : ::inet::TagBase(other)
{
    copy(other);
}

MetaTag::~MetaTag()
{
}

MetaTag& MetaTag::operator=(const MetaTag& other)
{
    if (this == &other) return *this;
    ::inet::TagBase::operator=(other);
    copy(other);
    return *this;
}

void MetaTag::copy(const MetaTag& other)
{
    this->pid = other.pid;
    this->mid = other.mid;
    this->src = other.src;
    this->dst = other.dst;
    this->numBytes = other.numBytes;
    this->ts = other.ts;
    this->tos = other.tos;
}

void MetaTag::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::TagBase::parsimPack(b);
    doParsimPacking(b,this->pid);
    doParsimPacking(b,this->mid);
    doParsimPacking(b,this->src);
    doParsimPacking(b,this->dst);
    doParsimPacking(b,this->numBytes);
    doParsimPacking(b,this->ts);
    doParsimPacking(b,this->tos);
}

void MetaTag::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::TagBase::parsimUnpack(b);
    doParsimUnpacking(b,this->pid);
    doParsimUnpacking(b,this->mid);
    doParsimUnpacking(b,this->src);
    doParsimUnpacking(b,this->dst);
    doParsimUnpacking(b,this->numBytes);
    doParsimUnpacking(b,this->ts);
    doParsimUnpacking(b,this->tos);
}

int MetaTag::getPid() const
{
    return this->pid;
}

void MetaTag::setPid(int pid)
{
    this->pid = pid;
}

int MetaTag::getMid() const
{
    return this->mid;
}

void MetaTag::setMid(int mid)
{
    this->mid = mid;
}

int MetaTag::getSrc() const
{
    return this->src;
}

void MetaTag::setSrc(int src)
{
    this->src = src;
}

int MetaTag::getDst() const
{
    return this->dst;
}

void MetaTag::setDst(int dst)
{
    this->dst = dst;
}

int MetaTag::getNumBytes() const
{
    return this->numBytes;
}

void MetaTag::setNumBytes(int numBytes)
{
    this->numBytes = numBytes;
}

double MetaTag::getTs() const
{
    return this->ts;
}

void MetaTag::setTs(double ts)
{
    this->ts = ts;
}

int MetaTag::getTos() const
{
    return this->tos;
}

void MetaTag::setTos(int tos)
{
    this->tos = tos;
}

class MetaTagDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_pid,
        FIELD_mid,
        FIELD_src,
        FIELD_dst,
        FIELD_numBytes,
        FIELD_ts,
        FIELD_tos,
    };
  public:
    MetaTagDescriptor();
    virtual ~MetaTagDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(MetaTagDescriptor)

MetaTagDescriptor::MetaTagDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(inet::MetaTag)), "inet::TagBase")
{
    propertyNames = nullptr;
}

MetaTagDescriptor::~MetaTagDescriptor()
{
    delete[] propertyNames;
}

bool MetaTagDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<MetaTag *>(obj)!=nullptr;
}

const char **MetaTagDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *MetaTagDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int MetaTagDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 7+base->getFieldCount() : 7;
}

unsigned int MetaTagDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_pid
        FD_ISEDITABLE,    // FIELD_mid
        FD_ISEDITABLE,    // FIELD_src
        FD_ISEDITABLE,    // FIELD_dst
        FD_ISEDITABLE,    // FIELD_numBytes
        FD_ISEDITABLE,    // FIELD_ts
        FD_ISEDITABLE,    // FIELD_tos
    };
    return (field >= 0 && field < 7) ? fieldTypeFlags[field] : 0;
}

const char *MetaTagDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "pid",
        "mid",
        "src",
        "dst",
        "numBytes",
        "ts",
        "tos",
    };
    return (field >= 0 && field < 7) ? fieldNames[field] : nullptr;
}

int MetaTagDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "pid") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "mid") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "src") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "dst") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "numBytes") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "ts") == 0) return baseIndex + 5;
    if (strcmp(fieldName, "tos") == 0) return baseIndex + 6;
    return base ? base->findField(fieldName) : -1;
}

const char *MetaTagDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_pid
        "int",    // FIELD_mid
        "int",    // FIELD_src
        "int",    // FIELD_dst
        "int",    // FIELD_numBytes
        "double",    // FIELD_ts
        "int",    // FIELD_tos
    };
    return (field >= 0 && field < 7) ? fieldTypeStrings[field] : nullptr;
}

const char **MetaTagDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *MetaTagDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int MetaTagDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    MetaTag *pp = omnetpp::fromAnyPtr<MetaTag>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void MetaTagDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    MetaTag *pp = omnetpp::fromAnyPtr<MetaTag>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'MetaTag'", field);
    }
}

const char *MetaTagDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    MetaTag *pp = omnetpp::fromAnyPtr<MetaTag>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string MetaTagDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    MetaTag *pp = omnetpp::fromAnyPtr<MetaTag>(object); (void)pp;
    switch (field) {
        case FIELD_pid: return long2string(pp->getPid());
        case FIELD_mid: return long2string(pp->getMid());
        case FIELD_src: return long2string(pp->getSrc());
        case FIELD_dst: return long2string(pp->getDst());
        case FIELD_numBytes: return long2string(pp->getNumBytes());
        case FIELD_ts: return double2string(pp->getTs());
        case FIELD_tos: return long2string(pp->getTos());
        default: return "";
    }
}

void MetaTagDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    MetaTag *pp = omnetpp::fromAnyPtr<MetaTag>(object); (void)pp;
    switch (field) {
        case FIELD_pid: pp->setPid(string2long(value)); break;
        case FIELD_mid: pp->setMid(string2long(value)); break;
        case FIELD_src: pp->setSrc(string2long(value)); break;
        case FIELD_dst: pp->setDst(string2long(value)); break;
        case FIELD_numBytes: pp->setNumBytes(string2long(value)); break;
        case FIELD_ts: pp->setTs(string2double(value)); break;
        case FIELD_tos: pp->setTos(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'MetaTag'", field);
    }
}

omnetpp::cValue MetaTagDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    MetaTag *pp = omnetpp::fromAnyPtr<MetaTag>(object); (void)pp;
    switch (field) {
        case FIELD_pid: return pp->getPid();
        case FIELD_mid: return pp->getMid();
        case FIELD_src: return pp->getSrc();
        case FIELD_dst: return pp->getDst();
        case FIELD_numBytes: return pp->getNumBytes();
        case FIELD_ts: return pp->getTs();
        case FIELD_tos: return pp->getTos();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'MetaTag' as cValue -- field index out of range?", field);
    }
}

void MetaTagDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    MetaTag *pp = omnetpp::fromAnyPtr<MetaTag>(object); (void)pp;
    switch (field) {
        case FIELD_pid: pp->setPid(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_mid: pp->setMid(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_src: pp->setSrc(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_dst: pp->setDst(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_numBytes: pp->setNumBytes(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_ts: pp->setTs(value.doubleValue()); break;
        case FIELD_tos: pp->setTos(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'MetaTag'", field);
    }
}

const char *MetaTagDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr MetaTagDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    MetaTag *pp = omnetpp::fromAnyPtr<MetaTag>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void MetaTagDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    MetaTag *pp = omnetpp::fromAnyPtr<MetaTag>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'MetaTag'", field);
    }
}

}  // namespace inet

namespace omnetpp {

}  // namespace omnetpp

