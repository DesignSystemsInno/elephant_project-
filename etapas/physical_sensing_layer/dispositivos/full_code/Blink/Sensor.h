
class Sensor{
    public:
        Sensor(uint8_t rx, uint8_t tx);
        void serialEvent();
        void dataSensor();
        int rx;
        int tx;
    
};
#endif

