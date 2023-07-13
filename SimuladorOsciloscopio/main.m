% Configuración
channel1Pin = 'A0';
channel2Pin = 'A1';
numSamples = 1000;  % Número de muestras a capturar

% Crear objeto de comunicación serie con Arduino
arduino = serialport("COM3", 9600);  % Reemplaza 'COMX' con el puerto correcto
configureTerminator(arduino, "LF");

% Capturar datos del canal 1
channel1Data = zeros(1, numSamples);
for i = 1:numSamples
    write(arduino, channel1Pin, "uint8");
    channel1Data(i) = str2double(readline(arduino));
end

% Capturar datos del canal 2
channel2Data = zeros(1, numSamples);
for i = 1:numSamples
    write(arduino, channel2Pin, "uint8");
    channel2Data(i) = str2double(readline(arduino));
end

% Cerrar conexión con Arduino
fclose(arduino);

% Visualizar las señales
figure;
subplot(2, 1, 1);
plot(channel1Data);
title('Canal 1 - Onda Senoidal');
xlabel('Muestra');
ylabel('Voltaje (V)');

subplot(2, 1, 2);
plot(channel2Data);
title('Canal 2 - Onda Cuadrada');
xlabel('Muestra');
ylabel('Voltaje (V)');
