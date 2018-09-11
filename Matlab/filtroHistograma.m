%<image_name> es el nombre del a imagen que vamos a procesar
image_name = 'DSC_0055.JPG'; %nombre de la imagen que vamos a tratar
hist_mean_filter = 11; %debe ser impar, filtro de histograma, minimo 1 maximo 256
filter_text = '';
img = imread(image_name);
[m,n] = size(img);
%Convertir a blanco y negro
img = rgb2gray(img);
bar_width = hist_mean_filter;
[shist, err] = sprintf('Tamaño de la ventana: %i\n',bar_width);
%Obtenemos el histograma de la imagen original
[counts,x]=imhist(img);
maxcounts = max(counts);
%Aplicamos un filtro de la media sobre el histograma
counts_filtered = smooth(counts,bar_width);
%Buscamos los mayores locales dentro de la imagen
fprintf('Identificando las clases (objetos) en la imagen\n');
[climbs,xclimbs] = FindGlobalMax(counts_filtered);
fprintf('Clases (objetos) identificadas');

clases = length(climbs); %numero de clases
fprintf('Numero de clases identificadas: %i\n',clases);
[sclases, err] = sprintf('*Numero de clases identificadas: %i\n *',clases);

fprintf('Buscando las fronteras por suma de medias\n');
[hills,xhills]=FindFrontiers(counts_filtered,xclimbs);
fprintf('Fronteras encontradas\n');
tit_s = 'Histograma con maximos y fronteras (Euclideas)';

figure
%Dibujamos el histograma
fprintf('Dibujamos el histograma identificando las clases encontradas\n');
color_clases = DrawHistClases(x,counts_filtered,xhills); %Dibujamos el histograma de clases
title(tit_s);
hold on;
%Dibujamos los maximos en el histograma
h = stem(xclimbs,climbs);
set(h,'Color','r','LineStyle','--','Marker','s','MarkerEdgeColor','r','MarkerFaceColor','r','MarkerSize',2);
axis([0 255 0 maxcounts+300]);
xlabel(strcat(sclases, shist));
hold off;
figure
%Utilizamos los colores que representan a cada clase
%para poder ser representados en una imagen de las mismas dimensiones de
%la imagen original donde podamos visualizara a que clase corresponde cada pixel en una imagen
fprintf('Identificando las clases en la imagen original\n');
img_clases = GetImgClases(img,xhills,color_clases); %imagen en nivel de gris,fronteras, colores para cada clase
fprintf('Clases identificadas...Dibujando\n');
imshow(img_clases);
title 'Imagen Segmentada'; 