#ifndef ISO8583_H
#define ISO8583_H

/*
ISO 8583  is an international standard  for financial transaction  card-originated  interchange  messaging.
It defines a messaging format and a communication flow for financial transactions between various entities,
such as point-of-sale (POS) terminals, automated teller machines (ATMs), and financial institutions. 

The  standard  was  developed  by  the  International  Organization  for  Standardization  (ISO) to  ensure
interoperability and consistency in electronic financial transactions.

Key features of ISO 8583 include:

-   Message Structure: ISO 8583 defines the structure of messages exchanged between different entities in a
    financial transaction.  The  message is typically a binary format with specific fields,  each serving a
    particular purpose in the transaction.

-   Message Types:  The  standard supports various types of financial transactions, such as  authorization,
    financial request,  reversal,  and reconciliation. Each message type has a specific purpose and set of
    data elements.

-   Data Elements: ISO 8583 specifies a set of data elements that make up a message. These elements include
    information such as transaction amount, cardholder account number, transaction date and time, and more.
    Each data element has a predefined format and length.

-   Transaction Processing:  The standard outlines the flow of a financial transaction,  from initiation to
    authorization and settlement. It defines how messages are exchanged between the involved parties and the
    actions to be taken at each stage of the transaction.

-   Security Features:   ISO 8583 includes provisions for securing financial transactions.  It supports the
    inclusion of security-related data elements to ensure the confidentiality and integrity of the transmitted
    information.

-   Network and Data Exchange:  The standard is designed to facilitate communication between different 
    components of a payment system, allowing for interoperability between devices, networks, and financial 
    institutions.

ISO 8583  has  been  widely  adopted in the financial industry and is a crucial component of many electronic
payment systems. It enables seamless communication between various entities involved in financial transactions,
ensuring that systems from different vendors can interoperate effectively. The standard has undergone several
revisions, with the latest version being ISO 8583:2013.

*/
#define PROTOCOL_NAME "ISO8583"
#define LATES_VERSION "2012"

/*
-   Header: Variable

-   Message Type Indicator (MTI): 4 bytes, identifies the message type.

-   Bitmap: 8 or 16 bytes, binary representation of included data elements.

-   Data Elements: Variable size, contains transaction information.

-   Secondary Bitmap (if needed): 8 bytes, extends data element range.

-   Subfields and Subelements: Variable size, nested details within data elements.

-   Trailer: Variable size, additional information (not always present).

It's important to note that ISO 8583 messages are typically  transmitted in a binary format for efficiency and
speed in financial transactions. The length of each field and the overall message structure are defined by the
standard, ensuring consistency and interoperability across different systems and institutions.
*/


extern char* MTI_LIST[10];
#endif