typedef enum colour {RED,BLUE} colour;

#define DEFAULT GREEN

#if ! defined DEFAULT
#error not defined
#endif

#if ! ((DEFAULT == RED) || (DEFAULT == BLUE))
#error true
#else
#error false
#endif
