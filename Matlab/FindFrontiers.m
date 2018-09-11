function[frontier,findex]=FindFrontiers(values,maxindex)
findex = zeros(1,length(maxindex)+1);
frontier = zeros(1,length(maxindex)+1);
findex(1)= 1;
frontier(1)= values(1);
for N=1:length(maxindex)-1
    findex(N+1) = round(maxindex(N) + (maxindex(N+1) - maxindex(N))/2);
    frontier(N+1) = values(findex(N+1));
end
findex(length(maxindex)+1)= 256; %Maximo valo
frontier(length(maxindex)+1) = values(256)