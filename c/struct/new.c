#include <stdio.h>

typedef struct objidv
{
  int x;
} objidv;

typedef struct any_type
{
  int x;
} any_type;

typedef int Int;
typedef int Uint16;
typedef int Sint16;
typedef int Sint32;
typedef int Octet;
typedef int Bool;
typedef int Length;

#define T_CTX 0
#define ObjectInstance_distinguishedName (T_CTX+2) /* tag */

/*
#include "general.h"
*/

/**/
typedef
struct AttributeValueAssertion {
  struct objidv attributeType;
  struct any_type attributeAssertionValue;
} AttributeValueAssertion;

typedef
struct RelativeDistinguishedName {
  Int n;
  struct AttributeValueAssertion v[1];
} RelativeDistinguishedName;

typedef
struct RDNSequence {
  Int n;
  struct RelativeDistinguishedName v[6];
} RDNSequence;

typedef
struct ObjectInstance {
  Uint16 t;
  union {
    struct RDNSequence distinguishedName;
    struct {
      Sint16 length;
      Octet value[16];
    } nonSpecificForm;
    struct RDNSequence localDistinguishedName;
  } v;
} ObjectInstance;

typedef
struct ToTPPools {
  Int n;
  struct {
    struct ObjectInstance tpPoolId;
    Sint16 numberOfTPs;
  } v[16];
} ToTPPools;

typedef
struct ExplicitTP {
  Uint16 t;
  union {
    struct ObjectInstance oneTPorGTP;
    struct {
      Int n;
      struct ObjectInstance v[1];
    } listofTPs;
  } v;
} ExplicitTP;

typedef
struct PtoMPools {
  struct ExplicitTP fromTp;
  struct ToTPPools toTPPools;
} PtoMPools;

typedef
struct ExplicitPtoMP {
  struct ExplicitTP fromTp;
  struct {
    Int n;
    struct ExplicitTP v[1];
  } toTPs;
} ExplicitPtoMP;

typedef
struct PtoTPPool {
  struct ExplicitTP fromTp;
  struct ObjectInstance toTpPool;
} PtoTPPool;

typedef
struct ExplicitPtoP {
  struct ExplicitTP fromTp;
  struct ExplicitTP toTP;
} ExplicitPtoP;

typedef
struct AddLeg {
  struct ObjectInstance mpCrossConnection;
  struct {
    Int n;
    struct {
      Uint16 t;
      union {
        struct ExplicitTP toTpOrGTP;
        struct ObjectInstance toPool;
      } v;
    } v[1];
  } legs;
} AddLeg;

typedef
struct ConnectInformation {
  Int n;
  struct {
    Uint16 m;
    struct {
      Uint16 t;
      union {
        struct {
          Uint16 t;
          union {
            struct ExplicitPtoP explicitPToP;
            struct PtoTPPool ptoTpPool;
            struct ExplicitPtoMP explicitPtoMP;
            struct PtoMPools ptoMPools;
          } v;
        } unidirectional;
        struct {
          Uint16 t;
          union {
            struct ExplicitPtoP explicitPToP;
            struct PtoTPPool ptoTpPool;
          } v;
        } bidirectional;
        struct AddLeg addleg;
      } v;
    } choice;
    Sint16 *administrativeState;
  } v[6];
} ConnectInformation;
/**/

static ConnectInformation CI;

void map ()
{
  ObjectInstance oi;

  oi.t = ObjectInstance_distinguishedName;
  oi.v.distinguishedName.n = 0;

  CI.v[0].choice.v.bidirectional.v.explicitPToP.toTp.v.oneTPorGTP = oi;
}

main()
{
  map();
}
