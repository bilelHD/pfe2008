 function parameter= wave2mfcc(y, fs, FP)
% wave2mfcc: Wave to MFCC (Mel-Frequency Cepstral Cofficient) conversion
%    Usage:
%    parameter = wave2mfcc(y, fs, FP)
%    parameter: MFCC and log energy, plus their delta value if necessary.
%    fs: sampling rate
%    FP: Parameters for deriving the speech features. You can use setFeatureParam.m to obtain the parameters.
%
%    For example:
%        waveFile='what_movies_have_you_seen_recently.wav';
%        [y, fs]=wavread(waveFile);
%        FP=setFeatureParam(fs);
%        FP.useDelta=0;
%        mfcc0= wave2mfcc(y, fs, FP);
%        fprintf('No. of extracted frames = %d\n', size(mfcc0, 2));
%        subplot(3,1,1); surf(mfcc0); box on; axis tight; title(sprintf('MFCC of "%s"', waveFile));
%        FP.useDelta=1;
%        mfcc1=wave2mfcc(y, fs, FP);
%        subplot(3,1,2); surf(mfcc1); box on; axis tight; title(sprintf('MFCC of "%s"', waveFile));
%        FP.useDelta=2;
%        mfcc2=wave2mfcc(y, fs, FP);
%        subplot(3,1,3); surf(mfcc2); box on; axis tight; title(sprintf('MFCC of "%s"', waveFile));

%    Roger Jang, 20070514

if nargin<1, selfdemo; return; end
if nargin<2; fs=8000; end
if nargin<3, FP=setFeatureParam(fs); end

y=double(y);    % Convert to double
%y=y-mean(y);    % Shift to zero mean

% ====== First Step : pre-emphasis.
%y = filter([1, -0.95], 1, y);

% ====== Second Step: frame blocking.

framedY = buffer2(y, FP.frameSize, FP.overlap);

%[fstart,fcenter,fstop] = getTriParam(FP.frameSize, fs, 0);    % Parameters for triangular filter bank
filterBankParam = getTriFilterParam(FP.frameSize, fs, FP.filterNum, 0);    % Parameters for triangular filter bank

parameter = [];
for i = 1:size(framedY, 2),
    % ====== Third Step: hamming window.
    Wframe  = hamming(FP.frameSize).*framedY(:,i);

    % ====== Forth Step: fast fourier transform.
    fftMag = abs(fft(framedY(:,i)));
    if (i == 1)
        %fprintf('%s\n', fftMag);
        %fprintf('FIN\n');
    end
    halfIndex = floor((FP.frameSize+1)/2);
    fftMag = fftMag(1:halfIndex);
    fftMag = interp1(1:halfIndex,fftMag,1:1/FP.alpha:halfIndex)';    % VTLN
    fftMag = [fftMag;zeros(halfIndex-length(fftMag),1)];

    % ====== Fifth Step: triangular bandpass filter.
%    tbfCoef = triBandFilter(fftMag, FP.filterNum, fstart, fcenter, fstop);
    tbfCoef = triBandFilter(fftMag, FP.filterNum, filterBankParam);

    % ====== Sixth Step: cosine transform. (Using DCT to get L order mel-scale-cepstrum parameters.)
    mfcc = melCepstrum(FP.mfccDim, FP.filterNum, tbfCoef);
    if (i == 62)
        fprintf('%s\n', mfcc);
    end

    parameter = [parameter mfcc'];
end

if (FP.useEnergyFeature==1)
    energy = sum(framedY.^2)/FP.frameSize;
    logEnergy = 10*log10(eps+energy);
    parameter = [parameter; logEnergy];
end

% ====== compute delta energy and delta cepstrum
% with delta is better for telephone digit recognition HMM
if (FP.useDelta>=1)
    deltaWindow = 2;
    paraDelta = deltaFunction(deltaWindow, parameter);
    parameter = [parameter; paraDelta];
end
if (FP.useDelta==2)
    paraDeltaDelta = deltaFunction(deltaWindow, paraDelta);
    parameter = [parameter; paraDeltaDelta];
end

% ====== Subfunction ======
% === Self demo
function selfdemo
waveFile='ane.wav';
[y, fs]=wavread(waveFile);
FP=setFeatureParam(fs);
FP.useDelta=0;
mfcc0 = wave2mfcc(y, fs, FP);
%mfcc0= feval(mfilename, y, fs, FP);
%fprintf('%s\n', mfcc0(1));
fprintf('No. of extracted frames = %d\n', size(mfcc0, 2));
%subplot(3,1,1); surf(mfcc0); box on; axis tight; title(sprintf('MFCC of "%s"', waveFile));
%FP.useDelta=1;
%mfcc1=feval(mfilename, y, fs, FP);
%subplot(3,1,2); surf(mfcc1); box on; axis tight; title(sprintf('MFCC of "%s"', waveFile));
%FP.useDelta=2;
%mfcc2=feval(mfilename, y, fs, FP);
%subplot(3,1,3); surf(mfcc2); box on; axis tight; title(sprintf('MFCC of "%s"', waveFile));

% === Triangular band-pass filters
function tbfCoef = triBandFilter(fftMag, P, filterBankParam)
fstart=filterBankParam(1,:);
fcenter=filterBankParam(2,:);
fstop=filterBankParam(3,:);
% Triangular bandpass filter.
for i=1:P

    %fprintf ('Pouet : start = %d  center = %d\n', fstart(i), fcenter(i));
   %fprintf ('#%d start = %d, center = %d, stop = %d\n', i, fstart(i), fcenter(i), fstop(i));


   for j = fstart(i):fcenter(i),
      filtmag(j) = (j-fstart(i))/(fcenter(i)-fstart(i));
   end
   for j = fcenter(i)+1:fstop(i),
      filtmag(j) = 1-(j-fcenter(i))/(fstop(i)-fcenter(i));
   end

   tbfCoef(i) = sum(fftMag(fstart(i):fstop(i)).*filtmag(fstart(i):fstop(i))');

   if (i == 1)
       %fprintf ('#%d (%d -> %d -> %d) filter_value = %d\n', i, fstart(i), fcenter(i), fstop(i), tbfCoef(i));
       %for k = fstart(i):fstop(i),
       %     fprintf ('%d * %d + ', fftMag (k), filtmag (k));
       %end
       %fprintf ('\n');
   end

end
   %fprintf('-------------------\n');

%tbfCoef=log(eps+tbfCoef.^2);

% === TBF coefficients to MFCC
function mfcc = melCepstrum(L, P, tbfCoef)
% DCT to find MFCC
for i = 1:L
   coef = cos((pi/P)*i*(linspace(1,P,P)-0.5))';
   mfcc(i) = sum(coef.*tbfCoef');
end


% === Delta function
function parameter = deltaFunction(deltaWindow,parameter)
% compute delta cepstrum and delta log energy.
rows  = size(parameter,1);
cols  = size(parameter,2);
%temp  = [zeros(rows,deltaWindow) parameter zeros(rows,deltaWindow)];
temp  = [parameter(:,1)*ones(1,deltaWindow) parameter parameter(:,end)*ones(1,deltaWindow)];
temp2 = zeros(rows,cols);
denominator = sum([1:deltaWindow].^2)*2;
for i = 1+deltaWindow : cols+deltaWindow,
   subtrahend = 0;
   minuend    = 0;
   for j = 1 : deltaWindow,
      subtrahend = subtrahend + temp(:,i+j)*j;
      minuend = minuend + temp(:,i-j)*(-j);
   end;
   temp2(:,i-deltaWindow) = (subtrahend + minuend)/denominator;
end;
parameter = temp2;

%==
  function FP=setFeatureParam(fs)
% setParam: Set parameters for feature extraction

if nargin<1; fs=8000; end

FP.useDelta = 0;        % 0(12fea), 1(24fea), 2(36fea)
FP.useEnergyFeature = 0;    %0,1
FP.upSampling = 2;        % 1, 2
FP.frameSize = 512;        % 128 <= 256,512 <= 1024
FP.overlapRatio = 1/2;        % 1/5 <= 1/4,1/3,1/2 <= 2/3
FP.useVTLN = 0;            %Vocal Track Length Normalization, 1 , 0
FP.useCMS = 1;            %Cepstral Mean Substraction, 0, 1(cms of all), 2(overlap(cms)= 24), 3(original+cms)
FP.testNum = 4;            % test sentence number, others is train sentences.
FP.alpha = 1;            % For VTLN
FP.frameSize = fs/(8000/256);    % Frame size
fprintf('FS = %d\n', FP.frameSize);
%FP.frameSize = 120;
FP.overlap = round(FP.frameSize/3);    % Frame overlap
FP.filterNum = 20;        % Number of triangular filter bank
FP.mfccDim = 20;        % Dimension of MFCC

function out = buffer2(y, frameSize, overlap)
% buffer2: Frame blocking
%    Usage: out = buffer2(y, frameSize, overlap)
%    This is almost the same as "buffer" except that there is no leading/trailing zeros

%    Roger Jang, 20010908


if nargin<3, overlap=0; end
if nargin<2, frameSize=256; end

y = y(:);
step = frameSize-overlap;
frameCount = floor((length(y)-overlap)/step);

out = zeros(frameSize, frameCount);
for i=1:frameCount,
    startIndex = (i-1)*step+1;
    if (i == 10)

        for j=startIndex:startIndex+frameSize-1,
            %fprintf ('#%d  %d\n', j, y(j));
        end
    end
    out(:, i) = y(startIndex:(startIndex+frameSize-1));
end

function filterBankParam = getTriFilterParam(frameSize, fs, filterNum, plotOpt)
% getTriParam: Get parameters of triangular filter bank

%    Roger Jang, 20030610

if nargin<1, selfdemo; return; end
if nargin<2, fs=8000; end
if nargin<3, filterNum=20; end
if nargin<4, plotOpt=0; end

maxMelFreq = freq2mel(fs/2);

sideWidth=maxMelFreq/(filterNum+1);

fprintf ('side_width = %d, maxMelFreq = %d\n', sideWidth, maxMelFreq);
index=0:filterNum-1;

%fprintf ('TEST\n');
%fprintf('%d\n', [index; index+1; index+2]);
%fprintf ('END TEST\n');

filterBankParam=floor(mel2freq([index; index+1; index+2]*sideWidth)/fs*frameSize)+1;

%fprintf ('DAAAAAAAAAAAAAAAAA 1\n');
%fprintf ('%d\n', filterBankParam);
%filterBankParam(end, end)= frameSize/2;    % ??????
%fprintf ('DAAAAAAAAAAAAAAAAA 2\n');
%fprintf ('%d\n', filterBankParam);
%fprintf ('DAAAAAAAAAAAAAAAAA 3\n');


if plotOpt
    subplot(2,1,1);
    plot(filterBankParam');
    xlabel('Filter bank indices');
    ylabel('Point indices');
    legend('Left indices', 'Center indices', 'Right indices');
    subplot(2,1,2); plot(filterBankParam, [0; 1; 0]);
    xlabel('Point indices');
end

% ====== Sub functions

% ====== Normal frequency to mel-scaled frequency conversion
function mel = freq2mel(freq)
mel = 2595*log10(1+freq/700);

% ====== Mel-scaled frequency to normal frequency conversion
function freq = mel2freq(mel)
freq = 700*(10.^(mel/2595)-1);
