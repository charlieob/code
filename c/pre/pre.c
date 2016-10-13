#define STRING XYZ

#if ! defined STRING
#error not defined
#endif

#if (STRING == ABC)
#error true
#else
#error false
#endif

main ()
{
}
