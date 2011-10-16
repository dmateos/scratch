class SessionsController < ApplicationController
  def new
    if signed_in?
      redirect_to current_user
      flash.now[:error] = "Already signed in"
    end
  end

  def create
    user = User.auth(params[:session][:email], params[:session][:password])
    if user.nil?
      flash.now[:error] = "Invalid email/password"
      render 'new'
    else
     sign_in user
     redirect_to '/'
    end
  end

  def destroy
    if signed_in?
      sign_out
      redirect_to '/'
    else
      flash.now[:error] = "You are not signed in"
    end
  end
end
