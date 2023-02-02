%% MICROCONTROLADORES
clear all
clc
warning('off','all')
UART0 = serialport("COM4",115200,"Timeout",60)
flush(UART0)
i = 1;
time = 0;
tmax = 249;

figure('Name','TIVA123')
l1=line(nan,nan,'Color','red');
l2=line(nan,nan,'Color','magenta');
l3=line(nan,nan,'Color','black');
l4=line(nan,nan,'Color','green');
l5=line(nan,nan,'Color','cyan');
l6=line(nan,nan,'Color','blue');
%l7=line(nan,nan,'Color','cyan');
%l8=line(nan,nan,'Color','magenta');
%l9=line(nan,nan,'Color','yellow');
% axes('XLim',[0 100],'YLim',[0 3.3]); 
grid on; hold on
tic

while(time<tmax)
    BT = readline(UART0);
    a=split(BT,',');
    b = (str2double(a).*3.3)./4095.0;
    v1(i)=b(1); v2(i)=b(2);v3(i)=b(3); v4(i)=b(4);v5(i)=b(5); v6(i)=b(6);
    %v7(i)=b(7);v8(i)=b(8); v9(i)=b(9);
    time=toc
    x=linspace(0,time,i);
    set(l1,'YData',v1(1:i),'XData',x);
    set(l2,'YData',v2(1:i),'XData',x);
    set(l3,'YData',v3(1:i),'XData',x);
    set(l4,'YData',v4(1:i),'XData',x);
    set(l5,'YData',v5(1:i),'XData',x);
    set(l6,'YData',v6(1:i),'XData',x);
    %set(l7,'YData',v7(1:i),'XData',x);
    %set(l8,'YData',v8(1:i),'XData',x);
    %set(l9,'YData',v9(1:i),'XData',x);
    hold on;
    drawnow;
    i = i+1;
end