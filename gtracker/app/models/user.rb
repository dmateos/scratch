require 'digest'

class User < ActiveRecord::Base
  attr_accessor :password
  attr_accessible :name, :email, :password, :password_confirmation, :salt

  validates_presence_of :name, :email
  validates_uniqueness_of :email
  validates :password,  :presence => true,
                        :confirmation => true,
                        :length => { :within => 6..40 } 

  before_save :encrypt_password 

  def has_password?(pass)
    return encrypted_password == encrypt(pass)
  end

  def self.auth(email, pass)
    user = find_by_email(email)
    return nil if user.nil?
    return user if user.has_password?(pass)
  end

  def self.auth_salt(id, salt)
    user = find_by_id(id)
    (user && user.salt == salt) ? user : nil
  end

  private
    def encrypt_password
      self.salt = make_salt if new_record?
      self.encrypted_password = encrypt(password)
    end

    def encrypt(string)
      secure_hash("#{salt}--#{string}")
    end

    def make_salt
      secure_hash("#{Time.now.utc}--#{password}")
    end

    def secure_hash(string)
      Digest::SHA2.hexdigest(string)
    end
end

