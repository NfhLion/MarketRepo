-- MySQL dump 10.13  Distrib 5.7.30, for Linux (x86_64)
--
-- Host: localhost    Database: market
-- ------------------------------------------------------
-- Server version	5.7.30-0ubuntu0.16.04.1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `adduserinfo`
--

DROP TABLE IF EXISTS `adduserinfo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `adduserinfo` (
  `add_qid` varchar(15) NOT NULL,
  `add_qname` varchar(30) NOT NULL,
  `add_qtype` varchar(10) NOT NULL,
  `add_userid` varchar(15) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `adduserinfo`
--

LOCK TABLES `adduserinfo` WRITE;
/*!40000 ALTER TABLE `adduserinfo` DISABLE KEYS */;
/*!40000 ALTER TABLE `adduserinfo` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `billinfo`
--

DROP TABLE IF EXISTS `billinfo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `billinfo` (
  `bill_id` varchar(15) NOT NULL,
  `bill_gid` varchar(15) NOT NULL,
  `bill_mid` varchar(15) NOT NULL,
  `bill_sale` varchar(20) DEFAULT NULL,
  `bill_return` varchar(20) DEFAULT NULL,
  PRIMARY KEY (`bill_id`),
  KEY `bgid` (`bill_gid`),
  KEY `bmid` (`bill_mid`),
  CONSTRAINT `bgid` FOREIGN KEY (`bill_gid`) REFERENCES `goodsinfo` (`goods_id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `bmid` FOREIGN KEY (`bill_mid`) REFERENCES `marketinfo` (`market_id`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `billinfo`
--

LOCK TABLES `billinfo` WRITE;
/*!40000 ALTER TABLE `billinfo` DISABLE KEYS */;
INSERT INTO `billinfo` VALUES ('BI10001','w10001','SHP10001','165600','37800'),('BI10002','w10002','SHP10001','212400','39600'),('BI10003','w10003','SHP10001','175000','30000'),('BI10004','w10004','SHP10001','795000','295000'),('BI10005','w10005','SHP10001','600000','150000'),('BI10006','w10001','SHP10002','230000','52500'),('BI10007','w10002','SHP10002','295000','55000'),('BI10008','w10006','SHP10002','6250000','1250000'),('BI10009','w10009','SHP10002','12640000','3840000'),('BI10010','w10010','SHP10003','24640000','4640000'),('BI10011','w10012','SHP10003','12640000','3840000'),('BI10012','w10008','SHP10003','13440000','2320000'),('BI10013','w10010','SHP10004','30800000','5800000'),('BI10014','w10001','SHP10005','115000','26250'),('BI10015','w10005','SHP10005','300000','75000'),('BI10016','w10010','SHP10005','20020000','3770000'),('BI10017','w10011','SHP10005','5040000','870000'),('BI10018','w10007','SHP10004','13860000','4410000'),('BI10019','w10015','SHP10004','3160000','960000'),('BI10020','w10014','SHP10004','2700000','575000'),('BI10021','w10013','SHP10004','9240000','1740000');
/*!40000 ALTER TABLE `billinfo` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `clientinfo`
--

DROP TABLE IF EXISTS `clientinfo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `clientinfo` (
  `client_id` varchar(15) NOT NULL,
  `client_name` varchar(30) NOT NULL,
  `client_sex` varchar(10) NOT NULL,
  `client_type` varchar(20) NOT NULL,
  `client_phone` varchar(15) DEFAULT NULL,
  `client_site` varchar(100) DEFAULT NULL,
  `client_head` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`client_id`),
  CONSTRAINT `client_id` FOREIGN KEY (`client_id`) REFERENCES `userinfo` (`user_id`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `clientinfo`
--

LOCK TABLES `clientinfo` WRITE;
/*!40000 ALTER TABLE `clientinfo` DISABLE KEYS */;
INSERT INTO `clientinfo` VALUES ('XIANXIA','XIANXIA','X','X','X','X','X');
/*!40000 ALTER TABLE `clientinfo` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `forminfo`
--

DROP TABLE IF EXISTS `forminfo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `forminfo` (
  `form_id` varchar(30) NOT NULL,
  `form_gid` varchar(15) NOT NULL,
  `form_cid` varchar(15) NOT NULL,
  `form_mid` varchar(15) NOT NULL,
  `form_num` varchar(20) NOT NULL,
  `form_price` varchar(20) NOT NULL,
  `form_state` varchar(10) NOT NULL,
  `form_time` varchar(50) DEFAULT NULL,
  `form_mode` varchar(10) DEFAULT NULL,
  `form_value` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`form_id`),
  KEY `gid` (`form_gid`),
  KEY `cid` (`form_cid`),
  KEY `mid` (`form_mid`),
  CONSTRAINT `cid` FOREIGN KEY (`form_cid`) REFERENCES `clientinfo` (`client_id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `gid` FOREIGN KEY (`form_gid`) REFERENCES `goodsinfo` (`goods_id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `mid` FOREIGN KEY (`form_mid`) REFERENCES `marketinfo` (`market_id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `forminfo`
--

LOCK TABLES `forminfo` WRITE;
/*!40000 ALTER TABLE `forminfo` DISABLE KEYS */;
INSERT INTO `forminfo` VALUES ('FM20200528150409','w10002','XIANXIA','SHP10001','2','10620','已支付','2020-05-28 15:04:09','微信支付','线下');
/*!40000 ALTER TABLE `forminfo` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `goods_numinfo`
--

DROP TABLE IF EXISTS `goods_numinfo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `goods_numinfo` (
  `goods_num_id` varchar(20) NOT NULL,
  `goods_num_gid` varchar(15) NOT NULL,
  `goods_num_mid` varchar(15) NOT NULL,
  `goods_num_all` varchar(20) DEFAULT NULL,
  `goods_num_sale` varchar(20) DEFAULT NULL,
  `goods_num_residue` varchar(20) DEFAULT NULL,
  `goods_num_shipment` varchar(20) DEFAULT NULL,
  PRIMARY KEY (`goods_num_id`),
  KEY `ngid` (`goods_num_gid`),
  KEY `nmid` (`goods_num_mid`),
  CONSTRAINT `ngid` FOREIGN KEY (`goods_num_gid`) REFERENCES `goodsinfo` (`goods_id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `nmid` FOREIGN KEY (`goods_num_mid`) REFERENCES `marketinfo` (`market_id`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `goods_numinfo`
--

LOCK TABLES `goods_numinfo` WRITE;
/*!40000 ALTER TABLE `goods_numinfo` DISABLE KEYS */;
INSERT INTO `goods_numinfo` VALUES ('WN10001','w10001','SHP10001','200','45','155','0'),('WN10002','w10002','SHP10001','100','38','62','0'),('WN10003','w10003','SHP10001','200','50','150','0'),('WN10004','w10004','SHP10001','260','50','210','0'),('WN10005','w10005','SHP10001','265','50','215','0'),('WN10006','w10001','SHP10002','250','50','200','0'),('WN10007','w10002','SHP10002','150','50','100','0'),('WN10008','w10006','SHP10002','140','50','90','0'),('WN10009','w10009','SHP10002','180','80','100','0'),('WN10010','w10010','SHP10003','180','80','100','0'),('WN10011','w10012','SHP10003','100','80','20','0'),('WN10012','w10008','SHP10003','200','80','120','0'),('WN10013','w10010','SHP10004','300','100','200','0'),('WN10014','w10001','SHP10005','100','25','75','0'),('WN10015','w10005','SHP10005','200','25','175','0'),('WN10016','w10010','SHP10005','100','65','35','0'),('WN10017','w10011','SHP10005','100','30','70','0'),('WN10018','w10007','SHP10004','200','70','130','0'),('WN10019','w10001','MER10001','1000','0','350','650'),('WN10020','w10002','MER10001','1000','0','750','250'),('WN10021','w10003','MER10001','1000','0','800','200'),('WN10022','w10004','MER10001','1000','0','740','260'),('WN10023','w10005','MER10001','1000','0','535','465'),('WN10024','w10006','MER10001','1000','0','860','140'),('WN10025','w10007','MER10001','1000','0','800','200'),('WN10026','w10009','MER10001','1000','0','710','290'),('WN10027','w10008','MER10001','1000','0','690','310'),('WN10028','w10010','MER10001','1000','0','360','640'),('WN10029','w10011','MER10001','1000','0','800','200'),('WN10030','w10012','MER10001','1000','0','900','100'),('WN10031','w10013','MER10001','1000','0','800','200'),('WN10032','w10014','MER10001','1000','0','850','150'),('WN10033','w10015','MER10001','1000','0','870','130'),('WN10034','w10015','SHP10004','100','20','80','0'),('WN10035','w10014','SHP10004','100','25','75','0'),('WN10036','w10013','SHP10004','100','30','70','0'),('WN20200519210740','w10007','SHP10001','50','0','50','0'),('WN20200519210929','w10013','SHP10001','100','0','100','0'),('WN20200525101120','w10001','SHP10006','100','0','100','0'),('WN20200525102650','w10009','SHP10006','100','0','100','0'),('WN20200525145701','w10009','SHP10001','10','0','10','0'),('WN20200526220212','w10015','SHP10001','30','0','30','0'),('WN20200526220710','w10014','SHP10001','50','0','50','0'),('WN20200526221142','w10010','SHP10002','60','0','60','0'),('WN20200526223614','w10008','SHP10001','50','0','50','0');
/*!40000 ALTER TABLE `goods_numinfo` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `goodsinfo`
--

DROP TABLE IF EXISTS `goodsinfo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `goodsinfo` (
  `goods_id` varchar(15) NOT NULL,
  `goods_name` varchar(50) NOT NULL,
  `goods_old_price` varchar(15) NOT NULL,
  `goods_new_price` varchar(15) NOT NULL,
  `goods_call_num` varchar(20) DEFAULT NULL,
  `goods_head` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`goods_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `goodsinfo`
--

LOCK TABLES `goodsinfo` WRITE;
/*!40000 ALTER TABLE `goodsinfo` DISABLE KEYS */;
INSERT INTO `goodsinfo` VALUES ('w10001','力洛克T006','3550','4600','8656','/home/python/science/MarketServer/watch_pic/watch/w10001-1.png'),('w10002','杜鲁尔T099','4800','5900','9655','/home/python/science/MarketServer/watch_pic/watch/w10002-1.png'),('w10003','TRAD-T063','2900','3500','10526','/home/python/science/MarketServer/watch_pic/watch/w10003-1.png'),('w10004','名匠L2','10000','15900','5256','/home/python/science/MarketServer/watch_pic/watch/w10004-1.png'),('w10005','瑰丽L4','9000','12000','9865','/home/python/science/MarketServer/watch_pic/watch/w10005-1.png'),('w10006','嘉岚L4','100000','125000','9867','/home/python/science/MarketServer/watch_pic/watch/w10006-1.png'),('w10007','马耳他82230','135000','198000','8867','/home/python/science/MarketServer/watch_pic/watch/w10007-1.png'),('w10008','历史名作3110V','139000','168000','5886','/home/python/science/MarketServer/watch_pic/watch/w10008-1.png'),('w10009','绿鬼116610','110000','158000','5886','/home/python/science/MarketServer/watch_pic/watch/w10009-1.png'),('w10010','日历型228238','250000','308000','6886','/home/python/science/MarketServer/watch_pic/watch/w10010-1.png'),('w10011','日历型经典款','139000','168000','8965','/home/python/science/MarketServer/watch_pic/watch/w10011-1.png'),('w10012','传承系列T002','110000','158000','10254','/home/python/science/MarketServer/watch_pic/watch/w10012-1.png'),('w10013','白金机械','250000','308000','1254','/home/python/science/MarketServer/watch_pic/watch/w10013-1.png'),('w10014','航海家系列','85000','108000','1254','/home/python/science/MarketServer/watch_pic/watch/w10014-1.png'),('w10015','蝶飞系列','110000','158000','12540','/home/python/science/MarketServer/watch_pic/watch/w10015-1.png');
/*!40000 ALTER TABLE `goodsinfo` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `marketinfo`
--

DROP TABLE IF EXISTS `marketinfo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `marketinfo` (
  `market_id` varchar(15) NOT NULL,
  `market_name` varchar(15) NOT NULL,
  `market_phone` varchar(15) DEFAULT NULL,
  `market_site` varchar(100) DEFAULT NULL,
  `market_head` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`market_id`),
  CONSTRAINT `market_id` FOREIGN KEY (`market_id`) REFERENCES `userinfo` (`user_id`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `marketinfo`
--

LOCK TABLES `marketinfo` WRITE;
/*!40000 ALTER TABLE `marketinfo` DISABLE KEYS */;
INSERT INTO `marketinfo` VALUES ('MER10001','腕尚乐淘总部','1571024001','山西省太原市',''),('SHP10001','腕表连锁店1','13327464136','山西省运城市',''),('SHP10002','腕表连锁店2','1024222310','山西省太原市',''),('SHP10003','腕表连锁店3','1331024222','山西省忻州市',''),('SHP10004','腕表连锁店4','1571024222','山西省吕梁市',''),('SHP10005','腕表连锁店5','1351024222','山西省临汾市',''),('SHP10006','腕表连锁店6','1024222333','山西省大同市',''),('SHP10007','腕表连锁店7','山西省','123456789',''),('SHP10008','腕表连锁店8','123456789','太原','');
/*!40000 ALTER TABLE `marketinfo` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `memberuserinfo`
--

DROP TABLE IF EXISTS `memberuserinfo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `memberuserinfo` (
  `member_id` varchar(25) NOT NULL,
  `member_username` varchar(30) NOT NULL,
  `member_phone` varchar(20) NOT NULL,
  `member_shopid` varchar(15) NOT NULL,
  `member_time` varchar(50) DEFAULT NULL,
  `member_grade` varchar(20) DEFAULT NULL,
  `member_total` varchar(20) DEFAULT NULL,
  `member_price` varchar(20) DEFAULT NULL,
  `member_discount` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`member_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `memberuserinfo`
--

LOCK TABLES `memberuserinfo` WRITE;
/*!40000 ALTER TABLE `memberuserinfo` DISABLE KEYS */;
INSERT INTO `memberuserinfo` VALUES ('MB20200525153301','张三','13327464136','SHP10001','2020-05-25 15:34:01','白金会员','2000','10620','0.9'),('MB20200525155102','李四','1024222310','SHP10001','2020-05-25 15:51:02','钻石会员','3000','7360','0.8'),('MEM20200526220135','小李','123456789','SHP10001','2020-05-26 22:01:33','白金会员','1000','4140','0.9');
/*!40000 ALTER TABLE `memberuserinfo` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `msginfo`
--

DROP TABLE IF EXISTS `msginfo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `msginfo` (
  `msg_id` varchar(20) NOT NULL,
  `msg_fid` varchar(15) NOT NULL,
  `msg_sid` varchar(15) NOT NULL,
  `msg_time` varchar(50) DEFAULT NULL,
  `msg_info` varchar(1000) DEFAULT NULL,
  `msg_size` varchar(20) DEFAULT NULL,
  `msg_state` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`msg_id`),
  KEY `msgfid` (`msg_fid`),
  KEY `msgsid` (`msg_sid`),
  CONSTRAINT `msgfid` FOREIGN KEY (`msg_fid`) REFERENCES `userinfo` (`user_id`) ON UPDATE CASCADE,
  CONSTRAINT `msgsid` FOREIGN KEY (`msg_sid`) REFERENCES `userinfo` (`user_id`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `msginfo`
--

LOCK TABLES `msginfo` WRITE;
/*!40000 ALTER TABLE `msginfo` DISABLE KEYS */;
INSERT INTO `msginfo` VALUES ('MSGR20200523103337','SHP10001','SHP10002','2020-05-23 10:33:37','大叔大婶','4','Y'),('MSGR20200523103405','SHP10002','SHP10001','2020-05-23 10:34:05','大叔大婶','4','Y'),('MSGR20200523151618','SHP10001','SHP10002','2020-05-23 15:35:34','再不','2','Y'),('MSGR20200523151638','SHP10001','SHP10002','2020-05-23 15:35:34','我给你发消息了，上线记得看一下','15','Y'),('MSGR20200523154008','SHP10001','SHP10002','2020-05-23 15:40:22','第二次测试','5','Y'),('MSGR20200523154013','SHP10001','SHP10002','2020-05-23 15:40:22','好的','2','Y'),('MSGR20200523154127','SHP10001','SHP10002','2020-05-23 15:41:47','终于快好了','5','Y'),('MSGR20200523154134','SHP10001','SHP10002','2020-05-23 15:41:47','太搞笑了。','5','Y'),('MSGR20200523154201','SHP10002','SHP10001','2020-05-23 15:42:26','知道了','3','Y'),('MSGR20200523154212','SHP10002','SHP10001','2020-05-23 15:42:26','值得祝贺一下','6','Y'),('MSGR20200524183143','SHP10001','SHP10002','2020-05-24 18:31:54','的撒打算','4','Y'),('MSGR20200524210626','SHP10006','SHP10001','2020-05-24 21:06:26','的撒大声地','5','Y'),('MSGR20200524210644','SHP10001','SHP10006','2020-05-24 21:06:43','哦哦哦','3','Y'),('MSGR20200525100908','MER10001','SHP10001','2020-05-25 10:09:08','在吗','2','Y'),('MSGR20200525100916','SHP10001','MER10001','2020-05-25 10:09:16','在','1','Y'),('MSGR20200525100920','MER10001','SHP10001','2020-05-25 10:09:20','好的','2','Y'),('MSGR20200525101026','SHP10006','MER10001','2020-05-25 10:10:26','你好','2','Y'),('MSGR20200525101037','MER10001','SHP10006','2020-05-25 10:10:37','好的，收到了','6','Y'),('MSGR20200525102822','MER10001','SHP10006','2020-05-25 10:28:22','打算的撒','4','Y'),('MSGR20200525102832','SHP10006','MER10001','2020-05-25 10:28:32','dasda ','6','Y'),('MSGR20200525144937','MER10001','SHP10006','2020-05-25 14:49:36','你是','2','Y'),('MSGR20200525144947','SHP10006','MER10001','2020-05-25 14:49:47','收到6号','4','Y');
/*!40000 ALTER TABLE `msginfo` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `shipmentinfo`
--

DROP TABLE IF EXISTS `shipmentinfo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `shipmentinfo` (
  `shipment_id` varchar(20) NOT NULL,
  `shipment_qmid` varchar(15) NOT NULL,
  `shipment_fmid` varchar(15) NOT NULL,
  `shipment_gid` varchar(15) NOT NULL,
  `shipment_num` varchar(20) NOT NULL,
  `shipment_time` varchar(100) NOT NULL,
  `shipment_state` varchar(10) NOT NULL,
  PRIMARY KEY (`shipment_id`),
  KEY `ship_qmid` (`shipment_qmid`),
  KEY `ship_fmid` (`shipment_fmid`),
  KEY `ship_gid` (`shipment_gid`),
  CONSTRAINT `ship_fmid` FOREIGN KEY (`shipment_fmid`) REFERENCES `marketinfo` (`market_id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `ship_gid` FOREIGN KEY (`shipment_gid`) REFERENCES `goodsinfo` (`goods_id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `ship_qmid` FOREIGN KEY (`shipment_qmid`) REFERENCES `marketinfo` (`market_id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `shipmentinfo`
--

LOCK TABLES `shipmentinfo` WRITE;
/*!40000 ALTER TABLE `shipmentinfo` DISABLE KEYS */;
INSERT INTO `shipmentinfo` VALUES ('SH20200513163601','SHP10001','MER10001','w10007','50','2020-05-19 21:07','已收到'),('SH20200513163602','SHP10001','MER10001','w10013','50','2020-05-19 21:09','已收到'),('SH20200513163603','SHP10001','MER10001','w10014','50','2020-05-26 22:07','已收到'),('SH20200513163604','SHP10002','MER10001','w10007','50','2020-05-13 16:41','审核中'),('SH20200513163605','SHP10003','MER10001','w10007','50','2020-05-13 16:42','审核中'),('SH20200513163606','SHP10004','MER10001','w10007','50','2020-05-13 16:43','审核中'),('SH20200513163607','SHP10005','MER10001','w10015','50','2020-05-13 16:45','审核中'),('SH20200518213448','SHP10001','MER10001','w10006','25','2020-05-19 16:04','已取消'),('SH20200518213617','SHP10001','MER10001','w10015','30','2020-05-26 22:02','已收到'),('SH20200518213801','SHP10001','MER10001','w10005','15','2020-05-26 22:25','已收到'),('SH20200518214053','SHP10003','MER10001','w10006','50','2020-05-18 21:40','审核中'),('SH20200518214611','SHP10003','MER10001','w10001','50','2020-05-18 21:46','审核中'),('SH20200518215625','SHP10004','MER10001','w10008','60','2020-05-19 09:51','调货中'),('SH20200518215843','SHP10004','MER10001','w10009','30','2020-05-18 21:58','审核中'),('SH20200518233212','SHP10001','MER10001','w10009','10','2020-05-25 14:57','已收到'),('SH20200518233327','SHP10003','MER10001','w10012','20','2020-05-18 23:33','审核中'),('SH20200518235927','SHP10001','MER10001','w10004','10','2020-05-26 22:26','已收到'),('SH20200518235951','SHP10001','MER10001','w10004','10','2020-05-26 22:26','已取消'),('SH20200519000114','SHP10002','MER10001','w10003','20','2020-05-21 22:57','已取消'),('SH20200519000252','SHP10002','MER10001','w10007','30','2020-05-26 22:11','已取消'),('SH20200519000803','SHP10002','MER10001','w10010','60','2020-05-26 22:11','已收到'),('SH20200519000850','SHP10002','MER10001','w10011','60','2020-05-19 16:10','已取消'),('SH20200519160937','SHP10002','MER10001','w10014','30','2020-05-19 16:10','已取消'),('SH20200519211837','SHP10004','MER10001','w10010','100','2020-05-19 21:18','已取消'),('SH20200519211858','SHP10004','MER10001','w10010','100','2020-05-19 21:21','已收到'),('SH20200519212023','SHP10004','MER10001','w10011','100','2020-05-19 21:21','调货中'),('SH20200520093829','SHP10001','MER10001','w10013','50','2020-05-26 22:27','已收到'),('SH20200525094312','SHP10001','MER10001','w10003','50','2020-05-26 22:26','已收到'),('SH20200525101053','SHP10006','MER10001','w10001','100','2020-05-25 10:11','已收到'),('SH20200525101622','SHP10006','MER10001','w10009','100','2020-05-25 10:26','已收到'),('SH20200525102553','SHP10006','MER10001','w10003','25','2020-05-25 10:27','已取消'),('SH20200526223543','SHP10001','MER10001','w10008','50','2020-05-26 22:36','已收到'),('SH20200530112153','SHP10001','MER10001','w10010','5','2020-05-30 11:21','审核中'),('SH20200530120525','SHP10001','MER10001','w10005','20','2020-05-30 12:05','审核中');
/*!40000 ALTER TABLE `shipmentinfo` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `userinfo`
--

DROP TABLE IF EXISTS `userinfo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `userinfo` (
  `user_id` varchar(15) NOT NULL,
  `user_passward` varchar(15) NOT NULL,
  `user_type` varchar(15) NOT NULL,
  `user_time` varchar(100) NOT NULL,
  `user_state` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `userinfo`
--

LOCK TABLES `userinfo` WRITE;
/*!40000 ALTER TABLE `userinfo` DISABLE KEYS */;
INSERT INTO `userinfo` VALUES ('MER10001','123456','market','2020-05-30 17:08:20','离线'),('SHP10001','123456','shop','2020-05-30 17:10:44','离线'),('SHP10002','123456','shop','2020-05-26 22:35:21','离线'),('SHP10003','123456','shop','2020-05-25 14:52:22','离线'),('SHP10004','123456','shop','2020-05-25 14:53:11','离线'),('SHP10005','123456','shop','2020-05-25 14:53:51','离线'),('SHP10006','123456','shop','2020-05-25 14:50:07','离线'),('SHP10007','123456','shop','2020-05-28 21:52:07','离线'),('SHP10008','123456','shop','2020-05-28 21:58:50','离线'),('XIANXIA','XIANXIA','XXX','XXX','线下支付');
/*!40000 ALTER TABLE `userinfo` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2020-05-30 17:22:25
