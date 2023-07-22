#include <math.h>

const unsigned int potAmplitud = A1;   ///< Pin para el potenciómetro que controla la amplitud de la señal.
const unsigned int potFrecuencia = A2; ///< Pin para el potenciómetro que controla la frecuencia de la señal.
const unsigned int potTipoSenal = A3;  ///< Pin para el potenciómetro que controla el tipo de señal.

const unsigned int ledAmarillo = 13;   ///< Pin para el LED amarillo que indica el canal 1 activo.
const unsigned int ledRojo = 12;       ///< Pin para el LED rojo que indica el canal 2 activo.

const unsigned int pulsanteCanal = 11; ///< Pin para el pulsador que alterna entre canales.

float tiempo = 0;                      ///< Tiempo utilizado para generar la señal de onda.

byte tipoSenal1 = 1;                   ///< Tipo de señal de onda para el canal 1 (1: seno, 2: cuadrada, 3: triangular, otros: diente de sierra).
byte tipoSenal2 = 2;                   ///< Tipo de señal de onda para el canal 2 (1: seno, 2: cuadrada, 3: triangular, otros: diente de sierra).

float amplitud1 = 1;                   ///< Amplitud de la señal de onda del canal 1.
float amplitud2 = 1;                   ///< Amplitud de la señal de onda del canal 2.

float frecuencia1 = 1;                 ///< Frecuencia de la señal de onda del canal 1.
float frecuencia2 = 1;                 ///< Frecuencia de la señal de onda del canal 2.

boolean canal = 0;                     ///< Variable para alternar entre los canales 1 y 2 mediante el pulsador.


/**
 * @brief Configuración inicial del programa.
 *
 * Se encarga de realizar la configuración inicial del hardware y comunicación necesaria para el funcionamiento del programa.
 *
 * Configura los siguientes elementos:
 * - Establece el pin 'ledAmarillo' como salida para controlar el LED amarillo.
 * - Establece el pin 'ledRojo' como salida para controlar el LED rojo.
 * - Establece el pin 'pulsanteCanal' como entrada para leer el estado del pulsador (botón).
 * - Inicia la comunicación serial (puerto serie) con una velocidad de transmisión de 9600 bps.
 *
 * Nota: La comunicación serial es útil para enviar datos desde el Arduino a un computador o para
 * interactuar con otros dispositivos que también utilicen comunicación serial.
 */
void setup() {

  pinMode(ledAmarillo, OUTPUT);
  pinMode(ledRojo, OUTPUT);

  pinMode(pulsanteCanal, INPUT);

  Serial.begin(9600);
}


/**
 * @brief Función principal que se ejecuta en bucle.
 *
 * Esta función se ejecuta en un bucle infinito y realiza las siguientes acciones:
 * - Lee el estado del pulsador (botón) conectado al pin 'pulsanteCanal' y cambia el valor de 'canal'
 *   entre 0 y 1 dependiendo del estado del pulsador.
 * - Si 'canal' es 0, realiza las siguientes acciones:
 *   - Lee los valores de tres potenciómetros: 'potTipoSenal', 'potAmplitud' y 'potFrecuencia',
 *     y los utiliza para actualizar los valores de 'tipoSenal1', 'amplitud1' y 'frecuencia1'
 *     respectivamente.
 *   - Enciende el LED amarillo (pin 'ledAmarillo') y apaga el LED rojo (pin 'ledRojo').
 * - Si 'canal' es 1, realiza las siguientes acciones:
 *   - Lee los valores de tres potenciómetros: 'potTipoSenal', 'potAmplitud' y 'potFrecuencia',
 *     y los utiliza para actualizar los valores de 'tipoSenal2', 'amplitud2' y 'frecuencia2'
 *     respectivamente.
 *   - Enciende el LED rojo (pin 'ledRojo') y apaga el LED amarillo (pin 'ledAmarillo').
 * - Genera dos señales de onda, una usando 'tipoSenal1', 'amplitud1', 'frecuencia1' y otra usando
 *   'tipoSenal2', 'amplitud2', 'frecuencia2'. Estas señales se calculan con la función 'getFormaDeOnda'.
 * - Imprime por el puerto serie (Serial) los valores de las dos señales generadas separados por una coma.
 * - Incrementa el valor de 'tiempo' en 0.005 segundos.
 * - Espera 10 milisegundos antes de repetir el bucle.
 */
void loop() {

  // Detecta si el pulsador (botón) conectado al pin 'pulsanteCanal' está presionado y cambia el valor de 'canal'
  // para alternar entre dos canales.
  if(digitalRead(pulsanteCanal) == 1)
    canal = !canal;

  if(canal == 0){
    // Canal 1
    tipoSenal1 = getlecturaPotenciometro(potTipoSenal,  1, 4, 1);
    amplitud1 = getlecturaPotenciometro(potAmplitud,  0, 100, 0.2);
    frecuencia1 = getlecturaPotenciometro(potFrecuencia, 1, 50, 1);
    digitalWrite(ledAmarillo, LOW);
    digitalWrite(ledRojo, HIGH);
  }else{
    // Canal 2
    tipoSenal2 = getlecturaPotenciometro(potTipoSenal,  1, 4, 1);
    amplitud2 = getlecturaPotenciometro(potAmplitud,  0, 100, 0.2);
    frecuencia2 = getlecturaPotenciometro(potFrecuencia, 1, 50, 1);
    digitalWrite(ledAmarillo, HIGH);
    digitalWrite(ledRojo, LOW);
  }

  // Genera dos señales de onda usando los valores obtenidos de los potenciómetros y la función 'getFormaDeOnda'.
  float senal1 = getFormaDeOnda(tipoSenal1, amplitud1, frecuencia1, tiempo);
  float senal2 = getFormaDeOnda(tipoSenal2, amplitud2, frecuencia2, tiempo);

  // Imprime por el puerto serie (Serial) los valores de las dos señales generadas, separados por una coma.
  Serial.println(String(senal1)+","+String(senal2));

  // Incrementa el valor de 'tiempo' en 0.005 segundos.
  tiempo += 0.005;

  // Espera 10 milisegundos antes de repetir el bucle.
  delay(10);
}


/**
 * Esta función permite generar diferentes formas de onda según el tipo especificado:
 * - Tipo 1: Onda senoidal.
 * - Tipo 2: Onda cuadrada con valores positivos y negativos.
 * - Tipo 3: Onda triangular.
 * - Otros tipos: Onda diente de sierra.
 *
 * @param tipo El tipo de forma de onda deseada (1: seno, 2: cuadrada, 3: triangular, otros: diente de sierra).
 * @param amplitudSenal La amplitud de la señal generada.
 * @param frecuenciaSenal La frecuencia de la señal generada en Hz.
 * @param tiempo El tiempo actual en segundos para generar la forma de onda.
 * @return El valor de la forma de onda en el tiempo especificado.
 */
float getFormaDeOnda(byte tipo, float amplitudSenal, float frecuenciaSenal, float tiempo){
  switch(tipo){
    case 1: 
      return amplitudSenal*sin(2*PI*tiempo*frecuenciaSenal);
    case 2: 
      return amplitudSenal*sign(sin(2*PI*frecuenciaSenal*tiempo));
    case 3:
      return (-amplitudSenal) + (2 * amplitudSenal / (1000.0 / frecuenciaSenal)) * (millis() % (unsigned long)(1000.0 / frecuenciaSenal));
    default:
      return (2.0 * amplitudSenal / (1000.0/frecuenciaSenal)) * (millis() % (unsigned long)(1000.0/frecuenciaSenal)) - amplitudSenal;
  }
}



/**
 * Esta función lee el valor analógico del potenciómetro conectado al pin especificado y lo mapea
 * a un rango personalizado definido por el valor mínimo, el número de incrementos y el tamaño del incremento.
 *
 * @param pinPotenciometro El pin analógico al cual está conectado el potenciómetro.
 * @param minValor El valor mínimo del rango deseado.
 * @param numIncrementos El número total de incrementos en el rango deseado.
 * @param incremento El tamaño del incremento entre cada valor del rango deseado.
 * @return El valor mapeado del potenciómetro en el rango personalizado.
 */
float getlecturaPotenciometro(unsigned int pinPotenciometro, unsigned int minValor, unsigned int numIncrementos, float incremento){
  unsigned int valorPot = analogRead(pinPotenciometro);
  float lectura = map(valorPot, 0, 1023, minValor, numIncrementos) * incremento;
  return lectura;
}


/**
 * Esta función toma un valor de tipo flotante y devuelve el signo correspondiente:
 * - Si el valor es positivo, devuelve 1.
 * - Si el valor es negativo, devuelve -1.
 * - Si el valor es cero, devuelve 0.
 *
 * @param valor El valor para el cual se desea obtener el signo.
 * @return El signo del valor: 1 si es positivo, -1 si es negativo, 0 si es cero.
 */
int sign(float valor){
  int resultado = 0;
  if(valor != 0)
    resultado = (valor < 0) ? -1 : 1;
  return resultado;
}