typedef enum Colour{

#define RED 0x0000
       XXMH_RED = RED,
#define BLUE 0x0001
       XXMH_BLUE = BLUE,
#define GREEN 0x0002
       XXMH_GREEN = GREEN
}Colour;

#define DEFAULT GREEN

#if ! defined DEFAULT
#error not defined
#endif

#if ! ((DEFAULT == RED) || (DEFAULT == BLUE))
#error true
#else
#error false
#endif

main(){

Colour colour;

colour = DEFAULT;
colour = RED;
colour = BLUE;
colour = GREEN;

}


