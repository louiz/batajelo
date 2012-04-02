SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL';

DROP SCHEMA IF EXISTS `batajelo` ;
CREATE SCHEMA IF NOT EXISTS `batajelo` DEFAULT CHARACTER SET utf8 ;
USE `batajelo` ;

-- -----------------------------------------------------
-- Table `batajelo`.`User`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `batajelo`.`User` ;

CREATE  TABLE IF NOT EXISTS `batajelo`.`User` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `login` VARCHAR(20) NOT NULL ,
  `password` VARCHAR(15) NOT NULL ,
  `is_banned` TINYINT(1) NULL DEFAULT 0 ,
  `is_online` TINYINT(1) NULL DEFAULT 0 ,
  `last_login` DATETIME NULL ,
  PRIMARY KEY (`id`) ,
  UNIQUE INDEX `login_UNIQUE` (`login` ASC) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `batajelo`.`User_Setting`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `batajelo`.`User_Setting` ;

CREATE  TABLE IF NOT EXISTS `batajelo`.`User_Setting` (
  `user_id` INT UNSIGNED NOT NULL ,
  `email` VARCHAR(60) NULL ,
  UNIQUE INDEX `mail_UNIQUE` (`email` ASC) ,
  INDEX `user_setting_user` (`user_id` ASC) ,
  CONSTRAINT `user_setting_user`
    FOREIGN KEY (`user_id` )
    REFERENCES `batajelo`.`User` (`id` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `batajelo`.`Request`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `batajelo`.`Request` ;

CREATE  TABLE IF NOT EXISTS `batajelo`.`Request` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `name` VARCHAR(45) NOT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `batajelo`.`User_Request`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `batajelo`.`User_Request` ;

CREATE  TABLE IF NOT EXISTS `batajelo`.`User_Request` (
  `request_id` INT UNSIGNED NOT NULL ,
  `sender_id` INT UNSIGNED NOT NULL ,
  `receiver_id` INT UNSIGNED NOT NULL ,
  INDEX `request_sender` (`sender_id` ASC) ,
  INDEX `request_receiver` (`receiver_id` ASC) ,
  INDEX `request_request` (`request_id` ASC) ,
  PRIMARY KEY (`request_id`, `sender_id`, `receiver_id`) ,
  CONSTRAINT `request_sender`
    FOREIGN KEY (`sender_id` )
    REFERENCES `batajelo`.`User` (`id` )
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `request_receiver`
    FOREIGN KEY (`receiver_id` )
    REFERENCES `batajelo`.`User` (`id` )
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `request_request`
    FOREIGN KEY (`request_id` )
    REFERENCES `batajelo`.`Request` (`id` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `batajelo`.`Achievement`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `batajelo`.`Achievement` ;

CREATE  TABLE IF NOT EXISTS `batajelo`.`Achievement` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `name` VARCHAR(45) NOT NULL ,
  `difficulty` ENUM('EASY','NORMAL','HARD') NOT NULL ,
  `description` TINYTEXT NOT NULL ,
  PRIMARY KEY (`id`) ,
  UNIQUE INDEX `name_UNIQUE` (`name` ASC) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `batajelo`.`User_Achievement`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `batajelo`.`User_Achievement` ;

CREATE  TABLE IF NOT EXISTS `batajelo`.`User_Achievement` (
  `user_id` INT UNSIGNED NOT NULL ,
  `achievement_id` INT UNSIGNED NOT NULL ,
  INDEX `achievementUser_user` (`user_id` ASC) ,
  INDEX `achievementUser_achievement` (`achievement_id` ASC) ,
  PRIMARY KEY (`user_id`, `achievement_id`) ,
  CONSTRAINT `achievementUser_user`
    FOREIGN KEY (`user_id` )
    REFERENCES `batajelo`.`User` (`id` )
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `achievementUser_achievement`
    FOREIGN KEY (`achievement_id` )
    REFERENCES `batajelo`.`Achievement` (`id` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `batajelo`.`Mode`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `batajelo`.`Mode` ;

CREATE  TABLE IF NOT EXISTS `batajelo`.`Mode` (
  `id` INT UNSIGNED NOT NULL ,
  `name` VARCHAR(60) NOT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `batajelo`.`Map`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `batajelo`.`Map` ;

CREATE  TABLE IF NOT EXISTS `batajelo`.`Map` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `name` VARCHAR(45) NOT NULL ,
  `description` VARCHAR(255) NOT NULL ,
  PRIMARY KEY (`id`) ,
  UNIQUE INDEX `name_UNIQUE` (`name` ASC) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `batajelo`.`Replay`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `batajelo`.`Replay` ;

CREATE  TABLE IF NOT EXISTS `batajelo`.`Replay` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `map_id` INT UNSIGNED NOT NULL ,
  `mode_id` INT UNSIGNED NOT NULL ,
  `name` VARCHAR(255) NOT NULL ,
  PRIMARY KEY (`id`) ,
  UNIQUE INDEX `name_UNIQUE` (`name` ASC) ,
  INDEX `replay_mode` (`mode_id` ASC) ,
  INDEX `replay_map` (`map_id` ASC) ,
  CONSTRAINT `replay_mode`
    FOREIGN KEY (`mode_id` )
    REFERENCES `batajelo`.`Mode` (`id` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE,
  CONSTRAINT `replay_map`
    FOREIGN KEY (`map_id` )
    REFERENCES `batajelo`.`Map` (`id` )
    ON DELETE NO ACTION
    ON UPDATE RESTRICT)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `batajelo`.`User_Replay`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `batajelo`.`User_Replay` ;

CREATE  TABLE IF NOT EXISTS `batajelo`.`User_Replay` (
  `replay_id` INT UNSIGNED NOT NULL ,
  `user_id` INT UNSIGNED NOT NULL ,
  INDEX `replayUser_replay` (`replay_id` ASC) ,
  INDEX `replayUser_user` (`user_id` ASC) ,
  PRIMARY KEY (`replay_id`, `user_id`) ,
  CONSTRAINT `replayUser_replay`
    FOREIGN KEY (`replay_id` )
    REFERENCES `batajelo`.`Replay` (`id` )
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `replayUser_user`
    FOREIGN KEY (`user_id` )
    REFERENCES `batajelo`.`User` (`id` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `batajelo`.`Map_Mode`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `batajelo`.`Map_Mode` ;

CREATE  TABLE IF NOT EXISTS `batajelo`.`Map_Mode` (
  `map_id` INT UNSIGNED NOT NULL ,
  `mode_id` INT UNSIGNED NOT NULL ,
  INDEX `mapMode_map` (`map_id` ASC) ,
  INDEX `mapMode_mode` (`mode_id` ASC) ,
  PRIMARY KEY (`map_id`, `mode_id`) ,
  CONSTRAINT `mapMode_map`
    FOREIGN KEY (`map_id` )
    REFERENCES `batajelo`.`Map` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `mapMode_mode`
    FOREIGN KEY (`mode_id` )
    REFERENCES `batajelo`.`Mode` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `batajelo`.`User_Friend`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `batajelo`.`User_Friend` ;

CREATE  TABLE IF NOT EXISTS `batajelo`.`User_Friend` (
  `user_id` INT UNSIGNED NOT NULL ,
  `friend_id` INT UNSIGNED NOT NULL ,
  INDEX `friendship_user` (`user_id` ASC) ,
  INDEX `friendship_friend` (`friend_id` ASC) ,
  PRIMARY KEY (`user_id`, `friend_id`) ,
  CONSTRAINT `friendship_user`
    FOREIGN KEY (`user_id` )
    REFERENCES `batajelo`.`User` (`id` )
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `friendship_friend`
    FOREIGN KEY (`friend_id` )
    REFERENCES `batajelo`.`User` (`id` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `batajelo`.`User_Ban`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `batajelo`.`User_Ban` ;

CREATE  TABLE IF NOT EXISTS `batajelo`.`User_Ban` (
  `user_id` INT UNSIGNED NOT NULL ,
  `ban_start` DATETIME NOT NULL ,
  `ban_end` DATETIME NOT NULL ,
  INDEX `ban_user_id` (`user_id` ASC) ,
  CONSTRAINT `ban_user_id`
    FOREIGN KEY (`user_id` )
    REFERENCES `batajelo`.`User` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;



SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;

-- -----------------------------------------------------
-- Data for table `batajelo`.`Request`
-- -----------------------------------------------------
START TRANSACTION;
USE `batajelo`;
INSERT INTO `batajelo`.`Request` (`id`, `name`) VALUES (1, 'friendship');

COMMIT;
