#include	<stdio.h>
#include	<stdlib.h>
#include	<iocslib.h>

int	arg[ 2 ] = { 1, 0 };

void	vdisp( void )
{
	while(  ( *(volatile unsigned char *)0xe88001 & 0x10 ) ) ;
	while( !( *(volatile unsigned char *)0xe88001 & 0x10 ) ) ;
}

void	vdisp2( void )
{
	int	i;
	for( i = 0; i < 20; i++ ) vdisp();
}
void	usage( void )
{
	fputs( "Usage:kbdmode.x [ n [ m ] ]\n", stdout );
	fputs( "\tn: Delay time at the start of repeat ( 200 + n * 100 [msec] )\n", stdout );
	fputs( "\tm: Subsequent repeat interval time ( 30 + m * 5 [msec] )\n", stdout );
	exit( 1 );
}

void	main( int argc, char **argv )
{
	int	i;
	unsigned char	led1, led2;

	fputs( "X68k Keyboard auto repeat speed changer by chika@kmc\n", stdout );

    B_SUPER( 0 );
	if( argc >= 4 || ( argc > 1 && argv[ 1 ][ 0 ] == '-' ) ) usage();

	for( i = 1 ; i < argc; i++ ) {
		arg[ i - 1 ] = atoi( argv[ i ] );
	}
	if( arg[ 0 ] < 0 || arg[ 0 ] > 15 ) usage();
	if( arg[ 1 ] < 0 || arg[ 1 ] > 15 ) usage();

	*(volatile unsigned char *)0xe8802f = 0x60 + arg[ 0 ];
	*(volatile unsigned char *)0xe8802d = 0x05;
	vdisp();

	*(volatile unsigned char *)0xe8802f = 0x70 + arg[ 1 ];
	*(volatile unsigned char *)0xe8802d = 0x05;
	vdisp();

	printf( "I set the keyboard repeat rate to:\n\n" );
	printf( "\tFirst %d [msec]\n", 200 + arg[ 0 ] * 100 );
	printf( "\tNext  %d [msec]\n", 30 + arg[ 1 ] * arg[ 1 ] * 5 );

	led1 = B_SFTSNS() / 256 ;
	led1 = led1 ^ 0xff | 0x80;
	led2 = led1 & 0xf0;

	*(volatile unsigned char *)0xe8802f = led2;
	*(volatile unsigned char *)0xe8802d = 0x05;
	vdisp2();
	*(volatile unsigned char *)0xe8802f = led1;
	*(volatile unsigned char *)0xe8802d = 0x05;
}
