#!/usr/bin/perl -w
#
# (c) 2005 by Ivan A. Alferov aka Plumbum
# e-mail: plumbum@rambler.ru
# Visit my page: http://dstar.dax.ru/
#

shift @ARG;

if ($inf = shift @ARG)
{
    unless (open(INF, $inf))
    {
        INF = STDIN;
    }
}
else
{
    INF = STDIN;
}

if ($ouf = shift @ARG)
{
    unless (open(OUF, ">$ouf"))
    {
        OUF = STDOUT;
    }
}
else
{
    OUF = STDOUT;
}


binmode INF;

$cnt = 0;
$online = 8;

print "\t";

while (eof(INF))
{
    read(INF, $buf, 1);
    print '0x', pack("hh", $buf), ", ";
    if ($cnt++ >= $online)
    {
        print OUF "\n\t";
    }
}

close OUF;
close INF;
