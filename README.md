## **`WaxLinker` EOSIO Smart Contract**

### **Description**:
The `WaxLinker` contract allows users to register their wallet IDs and link them to their Twitter IDs. The contract owner has special permissions to add or modify Twitter IDs and can also clear all entries from the contract.

### **Features**:

1. **addinfo**: 
   - Allows the contract owner to add or modify an entry with a given `user_id` and `twitterId`.
   - Ensures that the Twitter ID is unique based on its hash value.
   - Either creates a new entry or modifies an existing one.

2. **addwallet**: 
   - Allows a user to add their wallet ID.
   - Ensures the wallet ID is unique.

3. **remwallet**: 
   - Allows a user to remove their wallet ID.

4. **clearall**: 
   - Allows the contract owner to erase all entries from the `verifytab` table.

5. **hasentry**: 
   - Checks if a given `user_id` has an entry with a non-empty Twitter ID.
   - Prints "1" if the conditions are met and "0" otherwise.

### **Tables**:

1. **verifytab**: 
   - Stores information about the user's Twitter ID and its hash value.
   - Columns: 
     - `user_id`: Unique identifier for the user.
     - `twitterId`: Twitter ID of the user.
     - `hashValue`: A hashed value of the Twitter ID.
   - Indexes: 
     - Primary Key: `user_id`
     - Secondary Index: `hashValue`

2. **listtab**: 
   - Stores registered wallet IDs.
   - Columns:
     - `wallet_id`: Unique identifier for the user's wallet.
   - Indexes:
     - Primary Key: `wallet_id`

### **Utility Functions**:

1. **hash_string**: 
   - Hashes a given string using the SHA-256 algorithm.
   - Returns the first 8 bytes of the hash as a 64-bit integer.

2. **get_sender**: 
   - Retrieves the account name of the sender.

### **How It Works**:

- Users can register their wallet IDs using the `addwallet` action.
- The contract owner can add or modify Twitter IDs linked to a given wallet ID using the `addinfo` action.
- Users can remove their wallet IDs using the `remwallet` action.
- The contract owner can clear all data from the `verifytab` table using the `clearall` action.
- Anyone can check if a given wallet ID has a Twitter ID linked to it using the `hasentry` action.

### **Compilation and Deployment**:

#### **1. Compile the Contract**:
To compile the `verify` contract, use the following command:
```
eosio-cpp -abigen -o verify.wasm verify.cpp
```

#### **2. Set the Contract**:
To deploy the `verify` contract on the EOSIO blockchain, use the following command:
```
cleos -u https://api.waxsweden.org set contract xxxwaxlinker /Users/georgeprethesh/contracts/waxNFT verify.wasm --abi verify.abi -p xxxwaxlinker
```
Note: Replace `xxxwaxlinker` with the account name you wish to deploy the contract under.

