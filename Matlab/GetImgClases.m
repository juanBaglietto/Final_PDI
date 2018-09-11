function [img_clases] = GetImgClases(img,pos_clases,color_clases)
[M,N] = size(img);
img_clases = zeros(M,N,3); %Retorna una imagen a colores
for m=1:M
    for n=1:N
        pixel = img(m,n);
        %Un for para verificar si el punto pertenece a una clase en
        %particular
        for c=1:length(pos_clases-1)
            if(pixel >= pos_clases(c) && pixel < pos_clases(c+1))
                pcolor = color_clases(c,:);
            end
        end
        img_clases(m,n,:)=round(pcolor.*255);
    end
end
img_clases = cast(img_clases,'uint8');