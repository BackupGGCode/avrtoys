#!/usr/bin/perl -w
#

$sin_size = 256;

print <<EOF;
#define SIN_SIZE $sin_size

static prog_uint8_t sin[] =
    {
EOF

for($f = 0; $f < $sin_size; $f++)
{
	$r = 128+127*sin(rad($f));
	$intr = int($r+0.5);
	if(($f % 16) == 0) { print "\n\t"; }
	print $intr;
    print ", " if ($f != 359);
}

print "\n};\n";


sub rad
{
	my $p = shift;
	return ($p*3.1415926/($sin_size/2));
}

