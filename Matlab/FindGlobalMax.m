%Busca los maximos y minimos locales
function [max_arr,in] = FindGlobalMax(img_hist)
ant = img_hist(1);
sentido = 1; %1 sube y -1 baja
cont = 1;
max_arr(1) = 0;
in(1) = 0;
for N=1:length(img_hist)
    %fprintf('\nAnalizando %d \n', img_hist(N));
    %La recta no ha cambiado
    if(img_hist(N) ~= ant)
        if(ant > img_hist(N))
            if(sentido == 1)
                max_arr(cont) = ant;
                in(cont) = N-1;
                cont = cont + 1;
                sentido = -1;
                %fprintf('Maximo %d \n', img_hist(N));
            end
        else
            sentido = 1;
        end
    end
    ant = img_hist(N);
end