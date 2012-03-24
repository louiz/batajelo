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
  PRIMARY KEY (`user_id`) ,
  UNIQUE INDEX `mail_UNIQUE` (`email` ASC) ,
  INDEX `user_setting_user` (`user_id` ASC) ,
  CONSTRAINT `user_setting_user`
    FOREIGN KEY (`user_id` )
    REFERENCES `batajelo`.`User` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `batajelo`.`Request`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `batajelo`.`Request` ;

CREATE  TABLE IF NOT EXISTS `batajelo`.`Request` (
  `id` INT UNSIGNED NOT NULL ,
  `name` VARCHAR(45) NOT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `batajelo`.`User_Request`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `batajelo`.`User_Request` ;

CREATE  TABLE IF NOT EXISTS `batajelo`.`User_Request` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `sender_id` INT UNSIGNED NOT NULL ,
  `receiver_id` INT UNSIGNED NOT NULL ,
  `request_id` INT UNSIGNED NOT NULL ,
  PRIMARY KEY (`id`) ,
  INDEX `request_sender` (`sender_id` ASC) ,
  INDEX `request_receiver` (`receiver_id` ASC) ,
  INDEX `request_request` (`request_id` ASC) ,
  CONSTRAINT `request_sender`
    FOREIGN KEY (`sender_id` )
    REFERENCES `batajelo`.`User` (`id` )
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `request_receiver`
    FOREIGN KEY (`receiver_id` )
    REFERENCES `batajelo`.`User` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `request_request`
    FOREIGN KEY (`request_id` )
    REFERENCES `batajelo`.`Request` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
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
  CONSTRAINT `achievementUser_user`
    FOREIGN KEY (`user_id` )
    REFERENCES `batajelo`.`User` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `achievementUser_achievement`
    FOREIGN KEY (`achievement_id` )
    REFERENCES `batajelo`.`Achievement` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
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
    ON UPDATE NO ACTION,
  CONSTRAINT `replay_map`
    FOREIGN KEY (`map_id` )
    REFERENCES `batajelo`.`Map` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
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
  CONSTRAINT `replayUser_replay`
    FOREIGN KEY (`replay_id` )
    REFERENCES `batajelo`.`Replay` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `replayUser_user`
    FOREIGN KEY (`user_id` )
    REFERENCES `batajelo`.`User` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
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
  CONSTRAINT `friendship_user`
    FOREIGN KEY (`user_id` )
    REFERENCES `batajelo`.`User` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `friendship_friend`
    FOREIGN KEY (`friend_id` )
    REFERENCES `batajelo`.`User` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;



SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
