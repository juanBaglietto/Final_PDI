% a=uigetdir; %seleccionas la carpeta
% d = dir(a);
% c={};
% isub = [d(:).isdir];
% subcarpetas = {d(isub).name}';
% subcarpetas(ismember(subcarpetas,{'.','..'})) = [];
% for k=1:length(subcarpetas)
% f=dir(fullfile(a,subcarpetas{k},'*.jpg'));
% c=[c fullfile(a,subcarpetas{k},{f.name})];
% end


image_name = 'DSC_0060.JPG';
img = imread(image_name);
imshow(img);
figure(2);
[counts,x]=imhist(rgb2gray(img));
maxcounts = max(counts);
h = bar(x,counts);
set(h,'FaceColor',[102/255,153/255,102/255],'EdgeColor',[102/255,153/255,102/255]);
title 'Histograma Original';
axis([0 255 0 maxcounts+300]); 