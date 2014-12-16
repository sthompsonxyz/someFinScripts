#!/usr/bin/perl
use strict;
use warnings;

open(my $fh, "<", "../csvdata/MTFout.csv") or die "cannot open < MTFout.csv: $!";

#trade bot back test using MTF indicator as trend guide, boll band/stddev execution guide

#trade logic:
#MTF: 1D 1H 30min 15min 5min 1min
#boll indicator based on 20x1min +/- 1 std. dev.

#if 1H and 30min ++
  #if current 1min close comes within n of lower boll band
		#if flat then buy
		#short then buy
		#if long then hold
	#else if current 1 min close comes within n of upper boll band
		#if flat?
		#if short then hold
		#if long then sell
	#else 
		#if at prev 30min -+ (1 hour trend change)
			#if short then go long
			#if long then hold
			#if flat then buy
	#END

#else if 1H and 30min --
	#if current 1 min close comes within n of upper boll band
		#if flat then sell
		#if short then hold
		#if long then sell
	#else if current 1min close comes within n of lower boll band
		#if short then buy
		#if long then hold
		#if flat then hold
	#else
		#if at prev 30min +- (1 hour trend change)
			#if long then go short
			#if short then hold
			#if flat then sell
	#END
#END

#Globals
	#readline from csv and current price vars:
	my ($datetime, $datesecs, $open, $high, $low, $close, $count, $D1, $H1, $m30, $m15, $m5, $m1, $SMA, $uboll, $lboll);
	my $bollrange;
	#portfolio vars
	my ($position,$usdposition,$eurposition);
	#tradebot stat vars
	my $trades=0;

#EndGlobals

#initialise indicators

#initalise portfolio
$eurposition = 100000; #euros
$usdposition=0;
$position='flat';


#main bot loop
while(<$fh>)
{
	#read in current time state vars
	($datetime, $datesecs, $open, $high, $low, $close, $count, $D1, $H1, $m30, $m15, $m5, $m1, $SMA, $uboll, $lboll) = split(',', $_);
	#remove trailing \n from $lboll
	$lboll = substr($lboll,0,-1);
	#calc range between bollinger bands
	$bollrange = $uboll - $lboll;

	#trade logic:
	#MTF sets: 1D 1H 30min 15min 5min 1min
	#boll indicator based on 20x1min SMA

	#if 1H and 30min ++
	if(($H1 eq '+') and ($m1 eq '+'))
	{
		#if current 1min close comes within n of lower boll band, and range wide enough
		if(($close < ($lboll + 0.002) and ($bollrange > 0.001)))
		{
			#if flat then buy
			if($position eq 'flat')
			{
				($position,$eurposition,$usdposition) = flattolong($close,$eurposition,0.8);
				$trades++;
			}
			#else if short then go long
			elsif($position eq 'short')
			{
				($position,$eurposition,$usdposition) = shorttolong($close,$eurposition,$usdposition);
				$trades++;
			}	
			#else  (long) then hold
			else
			{
				#we're already long, do nothing 
			}
		}	
		#else if current 1 min close comes within n of upper boll band
		elsif(($close > ($uboll - 0.002) and ($bollrange > 0.002)))
		{
			#if flat?
			if($position eq 'flat')
			{
				##do nothing, don't open short pos in long higher timeframe trend...
			}
			#else if short then hold
			elsif($position eq 'short')
			{
				#hold short position, do nothing
			}
			#else (long) then sell
			else
			{
				($position,$eurposition,$usdposition) = longtoflat($close,$eurposition,$usdposition);
				$trades++;
			}
		}
		#else (other non boll MTF behaviour?)
		else
		{
			#if at prev 30min -+ (1 hour trend change)
	
				#if short then go long
				#if long then hold
				#if flat then buy
		}
		#END
	}
	#else if 1H and 30min --
	elsif(($H1 eq '-') and ($m1 eq '-'))
	{
		#if current 1 min close comes within n of upper boll band
		if(($close > ($uboll - 0.002) and ($bollrange > 0.004)))
		{
			#if flat then sell
			if($position eq 'flat')
			{
				($position,$eurposition,$usdposition) = flattoshort($close,$eurposition,0.8);
				$trades++;
			}
			#else if short then hold
			elsif($position eq 'short')
			{
				##do nothing
			}
			#else (long) then go short
			else
			{
				($position,$eurposition,$usdposition) = longtoshort($close,$eurposition,$usdposition);
				$trades++;
			}

		}
		#else if current 1min close comes within n of lower boll band
		elsif(($close < ($lboll + 0.002) and ($bollrange > 0.004)))
		{
			#if short then go long
			if($position eq 'short')
			{
				($position,$eurposition,$usdposition) = shorttolong($close,$eurposition,$usdposition);
				$trades++;
			}
			#else if long then hold
			elsif($position eq 'long')
			{
				##do nothing
			}
			#else (flat) then hold
			else
			{
				##do nothing
			}
		}
		#else
		else
		{
			#if at prev 30min +- (1 hour trend change)
				#if long then go short
				#if short then hold
				#if flat then sell
		}
		#END
	}
	#END
	else
	{

	}

#display state information at current time step
print STDOUT "$datetime,$close,$D1,$H1,$m30,$m15,$m5,$m1,$position,$eurposition,$usdposition,$trades\n";



}#END while <$fh>



sub flattolong
{
	my ($price,$eur,$available) = @_;
	my $availableEur = $eur * $available;
	my $usd = 0- ($availableEur * $price);
	$eur = $eur + $availableEur;

	return ('long',$eur,$usd);
}

sub flattoshort
{
	my ($price,$eur,$available) = @_;
	my $availableEur = $eur * $available;
	my $usd = $availableEur * $price;
	$eur = $eur - $availableEur;

	return ('short',$eur,$usd);
}

sub longtoflat
{
	my ($price,$eur,$usd) = @_;
	$eur = $eur + ($usd/$price);
	$usd = 0;
	return ('flat',$eur,$usd);
}

sub shorttoflat
{
	my ($price,$eur,$usd) = @_;
	$eur = $eur + ($usd/$price);
	$usd = 0;
	return ('flat',$eur,$usd);
}

sub longtoshort
{
	my ($price,$eur,$usd) = @_;
	my $pos;
	($pos,$eur,$usd) = longtoflat($price,$eur,$usd);
	($pos,$eur,$usd) = flattoshort($price,$eur,0.8);
	return ($pos,$eur,$usd);
}

sub shorttolong
{
	my ($price,$eur,$usd) = @_;
	my $pos;
	($pos,$eur,$usd) = shorttoflat($price,$eur,$usd);
	($pos,$eur,$usd) = flattolong($price,$eur,0.8);
	return ($pos,$eur,$usd);
}