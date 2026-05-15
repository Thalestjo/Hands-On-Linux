// SmartLamp Firmware - ESP32
// Protocolo Serial USB

// Defina os pinos de LED e LDR
int ledPin = 22;          // LED onboard (pode alterar conforme seu circuito)
int ldrPin = 34;         // Pino analógico do LDR (ADC ESP32)

// Valor atual do LED (0-100)
int ledValue = 10;

// Valor máximo do LDR (ajuste conforme testes reais)
int ldrMax = 4000;

// Threshold padrão
int thresholdValue = 50;

// Controle de envio periódico
unsigned long lastLdrSend = 0;
const unsigned long interval = 2000; // 2 segundos

void setup() {
    Serial.begin(9600);

    pinMode(ledPin, OUTPUT);
    pinMode(ldrPin, INPUT);

    ledUpdate();

    Serial.printf("SmartLamp Initialized.\n");
}

// Função loop será executada infinitamente pelo ESP32
void loop() {
    // Processa comandos da serial
    if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');
        command.trim(); // remove espaços e \r
        processCommand(command);
    }

    // Envio automático do LDR a cada 2 segundos
    if (millis() - lastLdrSend >= interval) {
        lastLdrSend = millis();

        int ldrValue = ldrGetValue();

        // Controle automático por threshold
        if (ldrValue < thresholdValue) {
            ledValue = 100;
        } else {
            ledValue = 0;
        }

        ledUpdate();

        Serial.print("RES GET_LDR ");
        Serial.println(ldrValue);
    }
}

void processCommand(String command) {

    // SET_LED X
    if (command.startsWith("SET_LED ")) {
        int value = command.substring(8).toInt();

        if (value >= 0 && value <= 100) {
            ledValue = value;
            ledUpdate();

            Serial.println("RES SET_LED 1");
        } else {
            Serial.println("RES SET_LED -1");
        }
    }

    // GET_LED
    else if (command == "GET_LED") {
        Serial.print("RES GET_LED ");
        Serial.println(ledValue);
    }

    // GET_LDR
    else if (command == "GET_LDR") {
        int ldrValue = ldrGetValue();

        Serial.print("RES GET_LDR ");
        Serial.println(ldrValue);
    }

    // SET_THRESHOLD X
    else if (command.startsWith("SET_THRESHOLD ")) {
        int value = command.substring(14).toInt();

        if (value >= 0 && value <= 100) {
            thresholdValue = value;

            Serial.println("RES SET_THRESHOLD 1");
        } else {
            Serial.println("RES SET_THRESHOLD -1");
        }
    }

    // GET_THRESHOLD
    else if (command == "GET_THRESHOLD") {
        Serial.print("RES GET_THRESHOLD ");
        Serial.println(thresholdValue);
    }

    // Comando inválido
    else {
        Serial.println("ERR Unknown command.");
    }
}

// Função para atualizar o valor do LED
void ledUpdate() {
    // Converte de 0-100 para 0-255
    int pwmValue = map(ledValue, 0, 100, 0, 255);

    // Garante limites válidos
    pwmValue = constrain(pwmValue, 0, 255);

    analogWrite(ledPin, pwmValue);
}

// Função para ler o valor do LDR
int ldrGetValue() {
    // Leitura bruta do sensor
    int rawValue = analogRead(ldrPin);

    // Garante limite máximo
    rawValue = constrain(rawValue, 0, ldrMax);

    // Normaliza para 0-100
    int normalizedValue = map(rawValue, 0, ldrMax, 0, 100);

    return normalizedValue;
}