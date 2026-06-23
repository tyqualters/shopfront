CREATE TABLE products (
	productId INT AUTO_INCREMENT PRIMARY KEY,
	productName VARCHAR(255),
	productPrice DECIMAL(6, 2),
	shopId INT,
	FOREIGN KEY (shopId) REFERENCES shops(shopId)
);
