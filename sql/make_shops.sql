CREATE TABLE shops (
	shopId INT AUTO_INCREMENT PRIMARY KEY,
	shopName VARCHAR(255),
	ownerId INT,
	FOREIGN KEY (ownerId) REFERENCES users(userId) 
);
