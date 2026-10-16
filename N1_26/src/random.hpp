class Random {
    public:
        Random() { }

        static const unsigned int generateRandomInt(
            unsigned int seed,
            unsigned int min,
            unsigned int max
        );
};