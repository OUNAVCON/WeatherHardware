#ifndef I2C_H


#define I2C_MASTER_IRQ I2C0_IRQn

#define EXAMPLE_I2C_MASTER_BASE I2C0_BASE

#define EXAMPLE_I2C_MASTER ((I2C_Type *)EXAMPLE_I2C_MASTER_BASE)

#if (EXAMPLE_I2C_MASTER_BASE == I2C0_BASE)
#define I2C_MASTER_CLK_SRC (I2C0_CLK_SRC)
#elif(EXAMPLE_I2C_MASTER_BASE == I2C1_BASE)
#define I2C_MASTER_CLK_SRC (I2C1_CLK_SRC)
#elif(EXAMPLE_I2C_MASTER_BASE == I2C2_BASE)
#define I2C_MASTER_CLK_SRC (I2C2_CLK_SRC)
#else
#error Should define the I2C_MASTER_CLK_SRC!
#endif




void I2C_init(void);

void I2C_Read(uint8_t addressOfSlave, /**< [in] Address of the slave( with offset?). */
		char numOfBytesToSend, /**< [in] Number of bytes to send to slave to start read. (1 based) */
		uint8_t* bytesToSend, /**< [in] pointer to the array to send. */
		char numBytesToRead, /**< [in] the number of bytes to read. */
		uint8_t* bytesToRead /**< [out] pointer to the array of bytes read from the slave. */
		);

#endif
