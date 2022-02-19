delete(instrfindall);
s1=serial('COM5')
s2=serial('COM8')
s2.OutputBufferSize = 3200000;
s2.Timeout=1000;
s2.Baudrate=9600;
fopen(s2);
fopen(s1);

while true
data=fscanf(s1) % Line from Arduino
x = reshape(dec2bin(data, 8).'-'0',1,[]); %binary 
bp=0.0001041666;  % bit period
bit=[]; 
for n=1:1:length(x)
    if x(n)==1;
       se=ones(1,100);
    else x(n)==0;
        se=zeros(1,100);
    end
     bit=[bit se];
end
t1=bp/100:bp/100:100*length(x)*(bp/100);
subplot(3,1,1);
plot(t1,bit,'lineWidth',2.5);grid on;
axis([ 0 bp*length(x) -.5 1.5]);
ylabel('amplitude(volt)');
xlabel('time(sec)');
title('binarni equivalent ');

%XXXXXXXXXXXXXXXXXXXXXXX Binary-FSK modulation XXXXXXXXXXXXXXXXXXXXXXXXXXX%
A=10;                                  % amplitude
br=1/bp;                               % speed of transmission
f1=br*30000;                           % frequency of carrier wave as 1
f2=br*10000;                           % frequency of carrier wave as 0
t2=bp/99:bp/99:bp;                 
ss=length(t2);
m=[];
for (i=1:1:length(x))
    if (x(i)==1)
        y=A*cos(2*pi*f1*t2);
    else
        y=A*cos(2*pi*f2*t2);
    end
    m=[m y]; 
end
m_str=mat2str(m(1:end/4));
pause(60);
fprintf(s2,m_str);
t3=bp/99:bp/99:bp*length(x);
subplot(3,1,2);
plot(t3,m);
xlabel('time(sec)');
ylabel('amplitude(volt)');
title('FSK signal');
%sound(m,20000)

end