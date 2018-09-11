function [colors] = DrawHistClases(x,counts,xhills)
%En base al numero de clases obtenemos los colores que vamos a usar para
%representar cada clase
clases = length(xhills)-1;
colors=zeros(clases,3);
colors(:,1) = rand(1,clases);
colors(:,2) = rand(1,clases);
colors(:,3) = rand(1,clases);

%Para colorearla de un color especifico la clase que corresponde a la
%puerta
clase = 3;
colors(clase,1) = 0.5;
colors(clase,2) = 1;
colors(clase,3) = 0.2;
%Voy dibujando poco a poco cada una de las clases
for c=1:length(xhills)-1
    x = (xhills(c):xhills(c+1));
    y = counts(xhills(c):xhills(c+1));
    hold on
    h=bar(x,y);
    set(h,'FaceColor',colors(c,:),'EdgeColor',colors(c,:));
end
hold off
