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
  PRIMARY KEY (`user_id`) ,
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
-- Table `batajelo`.`Team`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `batajelo`.`Team` ;

CREATE  TABLE IF NOT EXISTS `batajelo`.`Team` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `batajelo`.`Team_Achievement`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `batajelo`.`Team_Achievement` ;

CREATE  TABLE IF NOT EXISTS `batajelo`.`Team_Achievement` (
  `team_id` INT UNSIGNED NOT NULL ,
  `achievement_id` INT UNSIGNED NOT NULL ,
  INDEX `achievementTeam_team` (`team_id` ASC) ,
  INDEX `achievementTeam_achievement` (`achievement_id` ASC) ,
  PRIMARY KEY (`team_id`, `achievement_id`) ,
  CONSTRAINT `achievementTeam_team`
    FOREIGN KEY (`team_id` )
    REFERENCES `batajelo`.`Team` (`id` )
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `achievementTeam_achievement`
    FOREIGN KEY (`achievement_id` )
    REFERENCES `batajelo`.`Achievement` (`id` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
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
-- Table `batajelo`.`Mode`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `batajelo`.`Mode` ;

CREATE  TABLE IF NOT EXISTS `batajelo`.`Mode` (
  `id` INT UNSIGNED NOT NULL ,
  `name` VARCHAR(60) NOT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `batajelo`.`Map_Mode`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `batajelo`.`Map_Mode` ;

CREATE  TABLE IF NOT EXISTS `batajelo`.`Map_Mode` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `map_id` INT UNSIGNED NOT NULL ,
  `mode_id` INT UNSIGNED NOT NULL ,
  INDEX `mapMode_map` (`map_id` ASC) ,
  INDEX `mapMode_mode` (`mode_id` ASC) ,
  PRIMARY KEY (`id`, `map_id`, `mode_id`) ,
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
-- Table `batajelo`.`Season`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `batajelo`.`Season` ;

CREATE  TABLE IF NOT EXISTS `batajelo`.`Season` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `season_start` DATETIME NOT NULL ,
  `season_end` DATETIME NOT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `batajelo`.`Ladder`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `batajelo`.`Ladder` ;

CREATE  TABLE IF NOT EXISTS `batajelo`.`Ladder` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `name` VARCHAR(45) NOT NULL ,
  `description` TINYTEXT NOT NULL ,
  `size` INT UNSIGNED NOT NULL ,
  PRIMARY KEY (`id`) ,
  UNIQUE INDEX `name_UNIQUE` (`name` ASC) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `batajelo`.`Replay`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `batajelo`.`Replay` ;

CREATE  TABLE IF NOT EXISTS `batajelo`.`Replay` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `mapmode_id` INT UNSIGNED NOT NULL ,
  `season_id` INT UNSIGNED NOT NULL ,
  `ladder_id` INT UNSIGNED NOT NULL ,
  `name` VARCHAR(80) NOT NULL ,
  `date` DATETIME NOT NULL ,
  PRIMARY KEY (`id`) ,
  UNIQUE INDEX `name_UNIQUE` (`name` ASC) ,
  INDEX `replay_mapmode` (`mapmode_id` ASC) ,
  INDEX `replay_season` (`season_id` ASC) ,
  INDEX `replay_ladder` (`ladder_id` ASC) ,
  CONSTRAINT `replay_mapmode`
    FOREIGN KEY (`mapmode_id` )
    REFERENCES `batajelo`.`Map_Mode` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `replay_season`
    FOREIGN KEY (`season_id` )
    REFERENCES `batajelo`.`Season` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `replay_ladder`
    FOREIGN KEY (`ladder_id` )
    REFERENCES `batajelo`.`Ladder` (`id` )
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
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `batajelo`.`Map_Season`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `batajelo`.`Map_Season` ;

CREATE  TABLE IF NOT EXISTS `batajelo`.`Map_Season` (
  `map_id` INT UNSIGNED NOT NULL ,
  `mode_id` INT UNSIGNED NOT NULL ,
  `season_id` INT UNSIGNED NOT NULL ,
  PRIMARY KEY (`map_id`, `mode_id`, `season_id`) ,
  INDEX `mapSeason_map` (`map_id` ASC) ,
  INDEX `mapSeason_season` (`season_id` ASC) ,
  INDEX `mapSeason_mode` (`mode_id` ASC) ,
  CONSTRAINT `mapSeason_map`
    FOREIGN KEY (`map_id` )
    REFERENCES `batajelo`.`Map` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `mapSeason_season`
    FOREIGN KEY (`season_id` )
    REFERENCES `batajelo`.`Season` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `mapSeason_mode`
    FOREIGN KEY (`mode_id` )
    REFERENCES `batajelo`.`Mode` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `batajelo`.`Race`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `batajelo`.`Race` ;

CREATE  TABLE IF NOT EXISTS `batajelo`.`Race` (
  `id` INT UNSIGNED NOT NULL ,
  `name` VARCHAR(45) NOT NULL ,
  `description` TINYTEXT NOT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `batajelo`.`Team_User`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `batajelo`.`Team_User` ;

CREATE  TABLE IF NOT EXISTS `batajelo`.`Team_User` (
  `team_id` INT UNSIGNED NOT NULL ,
  `user_id` INT UNSIGNED NOT NULL ,
  `race_id` INT UNSIGNED NOT NULL ,
  `mode_id` INT UNSIGNED NOT NULL ,
  INDEX `userteam_team` (`team_id` ASC) ,
  INDEX `userteam_user` (`user_id` ASC) ,
  INDEX `userteam_race` (`race_id` ASC) ,
  INDEX `userteam_mode` (`mode_id` ASC) ,
  PRIMARY KEY (`team_id`, `user_id`, `race_id`, `mode_id`) ,
  CONSTRAINT `userteam_team`
    FOREIGN KEY (`team_id` )
    REFERENCES `batajelo`.`Team` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `userteam_user`
    FOREIGN KEY (`user_id` )
    REFERENCES `batajelo`.`User` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `userteam_race`
    FOREIGN KEY (`race_id` )
    REFERENCES `batajelo`.`Race` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `userteam_mode`
    FOREIGN KEY (`mode_id` )
    REFERENCES `batajelo`.`Mode` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `batajelo`.`Replay_Team`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `batajelo`.`Replay_Team` ;

CREATE  TABLE IF NOT EXISTS `batajelo`.`Replay_Team` (
  `replay_id` INT UNSIGNED NOT NULL ,
  `team_id` INT UNSIGNED NOT NULL ,
  `is_winner` TINYINT(1) NOT NULL DEFAULT 0 ,
  PRIMARY KEY (`replay_id`, `team_id`) ,
  INDEX `replayteam_replay` (`replay_id` ASC) ,
  INDEX `replayteam_team` (`team_id` ASC) ,
  CONSTRAINT `replayteam_replay`
    FOREIGN KEY (`replay_id` )
    REFERENCES `batajelo`.`Replay` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `replayteam_team`
    FOREIGN KEY (`team_id` )
    REFERENCES `batajelo`.`Team` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `batajelo`.`Team_Stats`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `batajelo`.`Team_Stats` ;

CREATE  TABLE IF NOT EXISTS `batajelo`.`Team_Stats` (
  `team_id` INT UNSIGNED NOT NULL ,
  `season_id` INT UNSIGNED NOT NULL ,
  `mode_id` INT UNSIGNED NOT NULL ,
  `race_id` INT UNSIGNED NOT NULL ,
  `ladder_id` INT UNSIGNED NOT NULL ,
  `rank` INT UNSIGNED NULL ,
  `win` INT UNSIGNED NOT NULL ,
  `loss` INT UNSIGNED NOT NULL ,
  PRIMARY KEY (`team_id`) ,
  INDEX `userStats_team` (`team_id` ASC) ,
  INDEX `userStats_season` (`season_id` ASC) ,
  INDEX `userStats_ladder` (`ladder_id` ASC) ,
  INDEX `userStats_mode` (`mode_id` ASC) ,
  INDEX `userStats_race` (`race_id` ASC) ,
  CONSTRAINT `userStats_team`
    FOREIGN KEY (`team_id` )
    REFERENCES `batajelo`.`Team` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `userStats_season`
    FOREIGN KEY (`season_id` )
    REFERENCES `batajelo`.`Season` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `userStats_ladder`
    FOREIGN KEY (`ladder_id` )
    REFERENCES `batajelo`.`Ladder` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `userStats_mode`
    FOREIGN KEY (`mode_id` )
    REFERENCES `batajelo`.`Mode` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `userStats_race`
    FOREIGN KEY (`race_id` )
    REFERENCES `batajelo`.`Race` (`id` )
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
