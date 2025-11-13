#include <unistd.h>

void	ft_putnbr(int nb);

int	main(void)
{
	write(1, "Print a positive number: ", 25);
	ft_putnbr(42);
	write(1, "\nPrint a negative number: ", 26);
	ft_putnbr(-42);
	write(1, "\nPrint zero: ", 13);
	ft_putnbr(0);
	write(1, "\nPrint the max value of int: ", 29);
	ft_putnbr(2147483647);
	write(1, "\nPrint the min value of int: ", 29);
	ft_putnbr(-2147483648);
	write(1, "\n", 1);
}
