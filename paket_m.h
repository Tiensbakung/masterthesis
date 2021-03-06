//
// Generated file, do not edit! Created by opp_msgc 4.4 from paket.msg.
//

#ifndef _PAKET_M_H_
#define _PAKET_M_H_

#include <omnetpp.h>

// opp_msgc version check
#define MSGC_VERSION 0x0404
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of opp_msgc: 'make clean' should help.
#endif

// cplusplus {{
#include "types.h"
// }}



/**
 * Class generated from <tt>paket.msg</tt> by opp_msgc.
 * <pre>
 * packet Paket {
 *        ipaddr src = 0;
 *        ipaddr dst = 0;
 *        vIndex index = 0;
 *        vSegment seg = 0;
 *        cValue v = 0;
 * }
 * </pre>
 */
class Paket : public ::cPacket
{
  protected:
    ipaddr src_var;
    ipaddr dst_var;
    vIndex index_var;
    vSegment seg_var;
    cValue v_var;

  private:
    void copy(const Paket& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const Paket&);

  public:
    Paket(const char *name=NULL, int kind=0);
    Paket(const Paket& other);
    virtual ~Paket();
    Paket& operator=(const Paket& other);
    virtual Paket *dup() const {return new Paket(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual ipaddr& getSrc();
    virtual const ipaddr& getSrc() const {return const_cast<Paket*>(this)->getSrc();}
    virtual void setSrc(const ipaddr& src);
    virtual ipaddr& getDst();
    virtual const ipaddr& getDst() const {return const_cast<Paket*>(this)->getDst();}
    virtual void setDst(const ipaddr& dst);
    virtual vIndex& getIndex();
    virtual const vIndex& getIndex() const {return const_cast<Paket*>(this)->getIndex();}
    virtual void setIndex(const vIndex& index);
    virtual vSegment& getSeg();
    virtual const vSegment& getSeg() const {return const_cast<Paket*>(this)->getSeg();}
    virtual void setSeg(const vSegment& seg);
    virtual cValue& getV();
    virtual const cValue& getV() const {return const_cast<Paket*>(this)->getV();}
    virtual void setV(const cValue& v);
};

inline void doPacking(cCommBuffer *b, Paket& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, Paket& obj) {obj.parsimUnpack(b);}


#endif // _PAKET_M_H_
