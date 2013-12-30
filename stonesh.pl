#!/usr/bin/perl -w
#
# stonesh.pl - the original stone shell
#
# usage:
#   - Add hosts to the array of allowed hosts.
#   - Set stonesh to be the user's default shell.

my @allowedhosts = ();
my @cmdline;

shift @ARGV;
@cmdline = split( /\s/, $ARGV[0], 2 ) if defined $ARGV[0];
if ( !defined $cmdline[0] || !grep { $_ eq $cmdline[0] } @allowedhosts ) {
    if ( !-t STDIN ) {
        print STDERR "You have no TTY and you have not specified a command";
        print STDERR "to be run on the remote host.  Try again with ssh -t";
        exit -1;
    }
    else {
        print "\n";
        print "Host: ";
        my $host = <STDIN>;
        chomp $host;
        if ( grep { $_ eq $host } @allowedhosts ) {
            @cmdline = ( $host, @ARGV );
        }
        else {
            print("Invalid host.\n");
            exit -1;
        }
    }
}

splice( @cmdline, 1, 0, '--' );
exec '/usr/bin/ssh', @cmdline;
