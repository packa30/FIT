% -----------------------------------------------------------
%	Projekt ISS, zdrojové kódy
%	Autor : Peter Grofèík		login : xgrofc00
% ------------------------------------------------------------
path(path,'c:/Users/peter/Desktop')% zmena cesty pre naèátanie signálu
signal='xgrofc00.wav';
[SIGNAL,Fs]=audioread(signal);
dlzka=length(SIGNAL);

spek=abs(fft(SIGNAL));
plot(Fs/2*linspace(0,1,dlzka/2+1),spek(1:dlzka/2+1));
title('Spectre');
xlabel('Hz');
ylabel('amp');

spek_max=find(max(spek)==spek)-1;

b=[0.2324 -0.4112 0.2324];
a=[1 0.2289 0.4662];
if_stab=roots(a);
zplane(b,a);
if (isempty(if_stab))
	disp('Not stable');
else if(abs(if_stab)>0)
	disp('Stable');
else
	disp('Not stable');
end
end

hertz=abs(freqz(b,a,256));
plot((0:255)/256*Fs/2,hertz);
xlabel('Hz'); 
ylabel('arg()');
title('F characteristics');

Clear_SIG=filter(b,a,SIGNAL);
Clear_dlzka=length(Clear_SIG);
spek_clear=abs(fft(Clear_SIG));
plot(Fs/2*linspace(0,1,Clear_dlzka/2+1),spek_clear(1:Clear_dlzka/2+1));
title('Spectre');
xlabel('Hz');
ylabel('amp');

spek_clear_max=find(max(spek_clear)==spek_clear)-1;

koef_k=-50:50;
plot(koef_k,xcorr(SIGNAL,50,'bias'));

koef_60=xcorr(SIGNAL,50,'bias');
disp(koef_60(61));