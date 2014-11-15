//
// Generated file, do not edit! Created by opp_msgc 4.4 from paket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "paket_m.h"

USING_NAMESPACE

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




Register_Class(Paket);

Paket::Paket(const char *name, int kind) : ::cPacket(name,kind)
{
    this->src_var = 0;
    this->dst_var = 0;
    this->index_var = 0;
    this->seg_var = 0;
    this->v_var = 0;
}

Paket::Paket(const Paket& other) : ::cPacket(other)
{
    copy(other);
}

Paket::~Paket()
{
}

Paket& Paket::operator=(const Paket& other)
{
    if (this==&other) return *this;
    ::cPacket::operator=(other);
    copy(other);
    return *this;
}

void Paket::copy(const Paket& other)
{
    this->src_var = other.src_var;
    this->dst_var = other.dst_var;
    this->index_var = other.index_var;
    this->seg_var = other.seg_var;
    this->v_var = other.v_var;
}

void Paket::parsimPack(cCommBuffer *b)
{
    ::cPacket::parsimPack(b);
    doPacking(b,this->src_var);
    doPacking(b,this->dst_var);
    doPacking(b,this->index_var);
    doPacking(b,this->seg_var);
    doPacking(b,this->v_var);
}

void Paket::parsimUnpack(cCommBuffer *b)
{
    ::cPacket::parsimUnpack(b);
    doUnpacking(b,this->src_var);
    doUnpacking(b,this->dst_var);
    doUnpacking(b,this->index_var);
    doUnpacking(b,this->seg_var);
    doUnpacking(b,this->v_var);
}

ipaddr& Paket::getSrc()
{
    return src_var;
}

void Paket::setSrc(const ipaddr& src)
{
    this->src_var = src;
}

ipaddr& Paket::getDst()
{
    return dst_var;
}

void Paket::setDst(const ipaddr& dst)
{
    this->dst_var = dst;
}

vIndex& Paket::getIndex()
{
    return index_var;
}

void Paket::setIndex(const vIndex& index)
{
    this->index_var = index;
}

vSegment& Paket::getSeg()
{
    return seg_var;
}

void Paket::setSeg(const vSegment& seg)
{
    this->seg_var = seg;
}

cValue& Paket::getV()
{
    return v_var;
}

void Paket::setV(const cValue& v)
{
    this->v_var = v;
}

class PaketDescriptor : public cClassDescriptor
{
  public:
    PaketDescriptor();
    virtual ~PaketDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(PaketDescriptor);

PaketDescriptor::PaketDescriptor() : cClassDescriptor("Paket", "cPacket")
{
}

PaketDescriptor::~PaketDescriptor()
{
}

bool PaketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<Paket *>(obj)!=NULL;
}

const char *PaketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int PaketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 5+basedesc->getFieldCount(object) : 5;
}

unsigned int PaketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
    };
    return (field>=0 && field<5) ? fieldTypeFlags[field] : 0;
}

const char *PaketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "src",
        "dst",
        "index",
        "seg",
        "v",
    };
    return (field>=0 && field<5) ? fieldNames[field] : NULL;
}

int PaketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "src")==0) return base+0;
    if (fieldName[0]=='d' && strcmp(fieldName, "dst")==0) return base+1;
    if (fieldName[0]=='i' && strcmp(fieldName, "index")==0) return base+2;
    if (fieldName[0]=='s' && strcmp(fieldName, "seg")==0) return base+3;
    if (fieldName[0]=='v' && strcmp(fieldName, "v")==0) return base+4;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *PaketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "ipaddr",
        "ipaddr",
        "vIndex",
        "vSegment",
        "cValue",
    };
    return (field>=0 && field<5) ? fieldTypeStrings[field] : NULL;
}

const char *PaketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int PaketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    Paket *pp = (Paket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string PaketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    Paket *pp = (Paket *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getSrc(); return out.str();}
        case 1: {std::stringstream out; out << pp->getDst(); return out.str();}
        case 2: {std::stringstream out; out << pp->getIndex(); return out.str();}
        case 3: {std::stringstream out; out << pp->getSeg(); return out.str();}
        case 4: {std::stringstream out; out << pp->getV(); return out.str();}
        default: return "";
    }
}

bool PaketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    Paket *pp = (Paket *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *PaketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        "ipaddr",
        "ipaddr",
        "vIndex",
        "vSegment",
        "cValue",
    };
    return (field>=0 && field<5) ? fieldStructNames[field] : NULL;
}

void *PaketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    Paket *pp = (Paket *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getSrc()); break;
        case 1: return (void *)(&pp->getDst()); break;
        case 2: return (void *)(&pp->getIndex()); break;
        case 3: return (void *)(&pp->getSeg()); break;
        case 4: return (void *)(&pp->getV()); break;
        default: return NULL;
    }
}


